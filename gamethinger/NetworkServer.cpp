#include "NetworkServer.h"

//#define _WINSOCK_DEPRECATED_NO_WARNINGS

//#ifndef ENET_IMPLEMENTATION
//	#define ENET_IMPLEMENTATION
//#endif // !#define ENET_IMPLEMENTATION

#include "enet/enet.h"

#include "NetworkHelpers.h"

ENetAddress server_address = { 0 };
ENetHost* server_server;

// the info we are tracking about each player in the game
typedef struct
{
	// is this player slot active
	bool Active;
	// the network connection they use
	ENetPeer* Peer;
}PlayerInfo;

PlayerInfo server_clients[MAX_CLIENTS];

int GetPlayerId(ENetPeer* peer)
{
	// find the slot that matches the pointer
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (server_clients->Active == true && server_clients[i].Peer == peer)
			return i;
	}
	return -1;
}

void NetworkServer::Connect(std::string ipAddress = NETWORK_IP, int port = NETWORK_PORT)
{
	// set up networking
	if (enet_initialize() != 0)
	{
		printf("[Server] enet_initialize failed\n");
		return;
	}

	// network servers must 'listen' on an interface and a port
	// this code sets up enet to listen on any available interface and using our port
	// the client must use the same port as the server and know the address of the server

	server_address.host = ENET_HOST_ANY;
	server_address.port = port;

	// create the server host
	server_server = enet_host_create(&server_address, MAX_CLIENTS, MAX_CHANNELS, 0, 0);

	if (server_server == NULL)
	{
		printf("[Server] enet_host_create failed\n");
		return;
	}

	printf("[Server] Started\n");
}

void NetworkServer::Disconnect()
{
	// cleanup
	enet_host_destroy(server_server);
	enet_deinitialize();
	printf("[Server] Shutdown\n");
}

void NetworkServer::Update(float dt)
{
	ENetEvent event;// = { 0 };

	// see if there are any inbound network events, wait up to 1000ms before returning.
	// if the server also did game logic, this timeout should be lowered
	while (enet_host_service(server_server, &event, 0) > 0)
	{
		// see what kind of event we have
		switch (event.type)
		{
		case ENET_EVENT_TYPE_NONE:
			// a player was disconnected
			//printf("[Server] Event NONE\n");
			if (OnNone != NULL)
				OnNone(event.packet->data, 0);
			break;
			// a new client is trying to connect
		case ENET_EVENT_TYPE_CONNECT:
		{
			//printf("[Server] Event CONNECT\n");

			// find an empty slot, or disconnect them if we are full
			int playerId = 0;
			for (; playerId < MAX_CLIENTS; playerId++)
			{
				if (!server_clients[playerId].Active) {

					//printf("[Server] Event CONNECT -- Have Empty Slot\n");
					break;
				}
			}

			// we are full
			if (playerId == MAX_CLIENTS)
			{
				// I said good day SIR!
				enet_peer_disconnect(event.peer, 0);
				//printf("[Server] Event CONNECT -- Server Full\n");
				break;
			}

			// player is good, don't give away the slot
			server_clients[playerId].Active = true;

			// but don't send out an update to everyone until they give us a good position
			//clients[playerId].ValidPosition = false;
			server_clients[playerId].Peer = event.peer;

			// pack up a message to send back to the client to tell them they have been accepted as a player
			uint8_t buffer[2] = { 0 };
			buffer[0] = (uint8_t)AcceptClient;  // command for the client
			buffer[1] = (uint8_t)playerId;    // the player ID so they know who they are

			// copy the buffer into an enet packet (TODO : add write functions to go directly to a packet)
			ENetPacket* packet = enet_packet_create(buffer, 2, ENET_PACKET_FLAG_RELIABLE);
			// send the data to the user
			enet_peer_send(event.peer, 0, packet);

			//printf("[Server] Event CONNECT -- Send Accept\n");

			if (OnConnect != NULL)
				OnConnect(event.packet->data, 0);
			break;
		}
		case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
			if (OnTimeout != NULL)
				OnTimeout(event.packet->data, 0);
		case ENET_EVENT_TYPE_DISCONNECT:
		{
			// a player was disconnected
			//printf("[Server] Event DISCONNECT\n");

			// find them if they are a real player
			int playerId = GetPlayerId(event.peer);
			if (playerId == -1)
				break;

			// mark them as inactive and clear the peer pointer
			server_clients[playerId].Active = false;
			server_clients[playerId].Peer = NULL;

			// Tell everyone that someone left
			size_t offset = 0;
			PackByte(packetBuffer, &offset, (uint8_t)NetworkCommands::RemoveClient);// command for the client
			PackInt(packetBuffer, &offset, playerId);
			// copy the buffer into an enet packet
			ENetPacket* packet = enet_packet_create(packetBuffer, offset, ENET_PACKET_FLAG_RELIABLE);
			// send the data to the user
			for (int i = 0; i < MAX_CLIENTS; i++)
			{
				if (!server_clients[i].Active)
					continue;
				enet_peer_send(server_clients[i].Peer, 0, packet);
			}

			if (OnDisconnect != NULL)
				OnDisconnect(event.packet->data, 0);
			break;
		}
		// someone sent us data
		case ENET_EVENT_TYPE_RECEIVE:
		{
			//printf("[Server] Event RECEIVE\n");
			// find the player who sent the data
			// we don't need them to send us what ID they are, we know who they are by the peer
			// we want to trust the client as little as possible so that people can't cheat/hack
			// if we blindly accepted a player ID, a client could send you updates for someone else :(

			int playerId = GetPlayerId(event.peer);
			if (playerId == -1)
			{
				// they are not one of our peeple, boot them
				enet_peer_disconnect(event.peer, 0);
				break;
			}

			// keep track of how far into the message we are
			size_t offset = 0;

			// read off the command the client wants us to process
			NetworkCommands command = (NetworkCommands)ReadByte(event.packet->data, &offset);

			// we only accept one message from clients for now, so make sure this is what it is
			if (command == NetworkCommands::Message)
			{
				int id = ReadInt(event.packet->data, &offset);
				std::string msg = ReadString(event.packet->data, &offset);
				std::cout << "[Server] Command:" << command << " clientID:" << id << " Size:" << msg.size() << " MSG:" << msg << std::endl;
				//relay the message
				SendToAll(msg);
			}
			else if (command == NetworkCommands::Update)
			{
				//int id = ReadInt(event.packet->data, &offset);
				//std::string msg = ReadString(event.packet->data, &offset);
				//std::cout << "Command:" << command << " clientID:" << id << " Size:" << msg.size() << " MSG:" << msg << std::endl;

				if (OnUpdate != NULL)
					OnUpdate(event.packet->data, offset);
			}
			//printf("[Server] Event RECEIVE -- End\n");
			break;
		}
		}//end switch

		// tell enet that it can recycle the inbound packet
		enet_packet_destroy(event.packet);
	}
}

bool NetworkServer::Connected()
{
	return server_server != NULL;
}

int NetworkServer::GetNetworkID()
{
	return networkID;
}

void NetworkServer::Send(std::string message)
{
	//dont need to send DM's so send to all
	SendToAll(message);
}

void NetworkServer::SendToAll(std::string message)
{
	size_t offset = 0;

	PackByte(packetBuffer, &offset, (uint8_t)NetworkCommands::Message);// command for the client
	PackInt(packetBuffer, &offset, networkID);
	PackString(packetBuffer, &offset, message);

	// copy the buffer into an enet packet
	ENetPacket* packet = enet_packet_create(packetBuffer, offset, ENET_PACKET_FLAG_RELIABLE);
	// send the data to the user
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (!server_clients[i].Active)
			continue;

		enet_peer_send(server_clients[i].Peer, 0, packet);
	}
}

void NetworkServer::Send(uint8_t data[], int dataSize)
{
	SendToAll(data, NULL);
}

void NetworkServer::SendTo(uint8_t data[], int dataSize, int networkID)
{
	size_t offset = 0;

	//uint8_t buffer[512] = { 0 };
	PackByte(packetBuffer, &offset, (uint8_t)NetworkCommands::Update);// command for the client
	PackInt(packetBuffer, &offset, networkID);// command for the client

	PackByteArray(packetBuffer, &offset, data, dataSize);

	// copy the buffer into an enet packet (TODO : add write functions to go directly to a packet)
	ENetPacket* packet = enet_packet_create(packetBuffer, offset, ENET_PACKET_FLAG_RELIABLE);

	//not real client
	if (networkID >= MAX_CLIENTS || networkID < 0)
		return;

	// send the data to the user
	if (!server_clients[networkID].Active)
		return;

	enet_peer_send(server_clients[networkID].Peer, 0, packet);
}

void NetworkServer::SendToAll(uint8_t data[], int dataSize)
{
	SendToAll(data, NULL);
}

void NetworkServer::SendToAll(uint8_t data[], int dataSize, int exclude)
{
	size_t offset = 0;

	//uint8_t buffer[512] = { 0 };
	PackByte(packetBuffer, &offset, (uint8_t)NetworkCommands::Update);// command for the client
	PackInt(packetBuffer, &offset, networkID);// command for the client

	PackByteArray(packetBuffer, &offset, data, dataSize);

	// copy the buffer into an enet packet (TODO : add write functions to go directly to a packet)
	ENetPacket* packet = enet_packet_create(packetBuffer, offset, ENET_PACKET_FLAG_RELIABLE);
	// send the data to the user

	if (exclude == NULL)
		exclude = -1;

	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (!server_clients[i].Active || i == exclude)
			continue;

		enet_peer_send(server_clients[i].Peer, 0, packet);
	}
}