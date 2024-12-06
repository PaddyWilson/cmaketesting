#include "NetworkClient.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define ENET_IMPLEMENTATION
#include <enet.h>

#include "NetworkHelpers.h"

// the enet address we are connected to
ENetAddress client_address = { 0 };

// the server object we are connecting to
ENetPeer* client_server = { 0 };

// the client peer we are using
ENetHost* client_client = { 0 };

void NetworkClient::Connect(std::string ipAddress = NETWORK_IP, int port = NETWORK_PORT)
{
	// startup the network library
	enet_initialize();

	// create a client that we will use to connect to the server
	client_client = enet_host_create(NULL, 1, MAX_CHANNELS, 0, 0);

	// set the address and port we will connect to
	enet_address_set_host(&client_address, ipAddress.c_str());
	client_address.port = port;

	// start the connection process. Will be finished as part of our update
	client_server = enet_host_connect(client_client, &client_address, 1, 0);

	//printf("[Client] Connect()\n");
}

void NetworkClient::Disconnect()
{
	// close our connection to the server
	if (client_server != NULL)
		enet_peer_disconnect_now(client_server, 0);

	// close our client
	if (client_client != NULL)
		enet_host_destroy(client_client);

	client_client = NULL;
	client_server = NULL;

	// clean up enet
	enet_deinitialize();

	//printf("[Client] Disconnect()\n");
}

void NetworkClient::Update(float dt)
{
	//LastNow = now;
	// if we are not connected to anything yet, we can't do anything, so bail out early
	if (client_server == NULL)
		return;

	// read one event from enet and process it
	ENetEvent Event;// = { 0 };

	// Check to see if we even have any events to do. Since this is a a client, we don't set a timeout so that the client can keep going if there are no events
	while (enet_host_service(client_client, &Event, 0) > 0)
	{
		// see what kind of event it is
		switch (Event.type)
		{
		case ENET_EVENT_TYPE_NONE:
			//printf("[Client] Event NONE\n");
			if (OnNone != NULL)
				OnNone(Event.packet->data, 0);

			break;
		case ENET_EVENT_TYPE_CONNECT:
			//printf("[Client] Event CONNECT\n");

			if (OnConnect != NULL)
				OnConnect(Event.packet->data, 0);

			break;
			// we were disconnected, we have a sad
		case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
			if (OnTimeout != NULL)
				OnTimeout(Event.packet->data, 0);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			//printf("[Client] Event DISCONNECT\n");
			client_server = NULL;
			networkID = -1;

			if (OnDisconnect != NULL)
				OnDisconnect(Event.packet->data, 0);
			break;

			// the server sent us some data, we should process it
		case ENET_EVENT_TYPE_RECEIVE:
		{
			//printf("[Client] Event RECEIVE\n");
			// we know that all valid packets have a size >= 1, so if we get this, something is bad and we ignore it.
			if (Event.packet->dataLength < 1)
				break;

			// keep an offset of what data we have read so far
			size_t offset = 0;

			// read off the command that the server wants us to do
			NetworkCommands command = (NetworkCommands)ReadByte(Event.packet->data, &offset);

			// if the server has not accepted us yet, we are limited in what packets we can receive
			if (networkID == -1)
			{
				if (command != AcceptClient) // this is the only thing we can do in this state, so ignore anything else
					break;

				// See who the server says we are
				this->networkID = ReadByte(Event.packet->data, &offset);

				// Make sure that it makes sense
				if (networkID < 0 || networkID > MAX_CLIENTS)
				{
					networkID = -1;
					break;
				}

				if (OnAccept != NULL)
					OnAccept(Event.packet->data, offset);

				//printf("[Client] Event RECEIVE -- Server Accepted\n");
			}
			else if (command == NetworkCommands::AddClient)
			{
				int id = ReadInt(Event.packet->data, &offset);
				if (OnAdd != NULL)
					OnAdd(Event.packet->data, offset);
			}
			else if (command == NetworkCommands::RemoveClient)
			{
				int id = ReadInt(Event.packet->data, &offset);
				if (OnRemove != NULL)
					OnRemove(Event.packet->data, offset);
			}
			else if(command == NetworkCommands::Message)
			{
				int id = ReadInt(Event.packet->data, &offset);
				std::string msg = ReadString(Event.packet->data, &offset);
				std::cout << "[Client] Command:" << command << " clientID:" << id << " Size:" << msg.size() << " MSG:" << msg << std::endl;

				if (OnMessage != NULL)
					OnMessage(Event.packet->data, offset);
			}
			else if (command == NetworkCommands::Update)
			{
				//int id = ReadInt(Event.packet->data, &offset);
				//std::string msg = ReadString(event.packet, &offset);
				//std::cout << "Command:" << command << " clientID:" << id << " Size:" << msg.size() << " MSG:" << msg << std::endl;

				if (OnUpdate != NULL)
					OnUpdate(Event.packet->data, offset);
			}

			break;
		}
		}//end switch

		// tell enet that it can recycle the packet data
		enet_packet_destroy(Event.packet);

	}//if has network event
}

bool NetworkClient::Connected()
{
	return client_server != NULL && networkID >= 0;
}

int NetworkClient::GetNetworkID()
{
	return networkID;
}

void NetworkClient::Send(std::string message)
{
	size_t offset = 0;

	PackByte(packetBuffer, &offset, (uint8_t)NetworkCommands::Message);// command for the client
	PackInt(packetBuffer, &offset, networkID);
	PackString(packetBuffer, &offset, message);

	// copy the buffer into an enet packet
	ENetPacket* packet = enet_packet_create(packetBuffer, offset, ENET_PACKET_FLAG_RELIABLE);
	// send the data to the user
	enet_peer_send(client_server, 0, packet);
}


void NetworkClient::SendToAll(std::string message)
{
	//client only sends to server
	Send(message);
}

void NetworkClient::Send(uint8_t data[], int dataSize)
{
	size_t offset = 0;

	//uint8_t buffer[512] = { 0 };
	PackByte(packetBuffer, &offset, (uint8_t)NetworkCommands::Update);// command for the client
	PackInt(packetBuffer, &offset, networkID);// command for the client

	PackByteArray(packetBuffer, &offset, data, dataSize);

	// copy the buffer into an enet packet
	ENetPacket* packet = enet_packet_create(packetBuffer, offset, ENET_PACKET_FLAG_RELIABLE);
	// send the data to the user
	enet_peer_send(client_server, 0, packet);
}

void NetworkClient::SendTo(uint8_t data[], int dataSize, int networkID)
{
	//client only sends to server
	Send(data, dataSize);
}

void NetworkClient::SendToAll(uint8_t data[], int dataSize)
{
	//client only sends to server
	Send(data, dataSize);
}

void NetworkClient::SendToAll(uint8_t data[], int dataSize, int exclude)
{
	//client only sends to server
	Send(data, dataSize);
}
