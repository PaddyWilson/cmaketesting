#pragma once

#include <string>
#include <vector>
#include <functional>

#include "NetworkHelpers.h"

#define NETWORK_IP "127.0.0.1"
#define NETWORK_PORT 20000

#define MAX_CLIENTS 50
#define MAX_CHANNELS 1

#define PACKET_BUFFER_SIZE 1000000 //1 MB

typedef enum
{
	// Server -> Client, You have been accepted. Contains the id for the client player to use
	AcceptClient = 1,

	// Server -> Client, Add a new player to your simulation, contains the ID of the player and a position
	AddClient = 2,

	// Server -> Client, Remove a player from your simulation, contains the ID of the player to remove
	RemoveClient = 3,

	// Server -> Client, Update a player's position in the simulation, contains the ID of the player and a position
	Update = 4,

	// Client -> Server, //used for initial testing
	Message = 5,
}NetworkCommands;

class NetworkBase
{
public:
	virtual void Connect(std::string ipAddress, int port) = 0;
	virtual void Disconnect() = 0;
	virtual void Update(float dt) = 0;

	virtual bool Connected() = 0;
	virtual int GetNetworkID() = 0;

	//don't realy need. was used for testing
	virtual void Send(std::string message) = 0;
	virtual void SendToAll(std::string message) = 0;

	virtual void Send(uint8_t data[], int dataSize) = 0;
	virtual void SendTo(uint8_t data[], int dataSize, int networkID) = 0;
	virtual void SendToAll(uint8_t data[], int dataSize) = 0;
	virtual void SendToAll(uint8_t data[], int dataSize, int exclude) = 0;

	std::function<void(uint8_t[], size_t)> OnNone = NULL;
	std::function<void(uint8_t*, size_t)> OnConnect = NULL;
	std::function<void(uint8_t*, size_t)> OnTimeout = NULL;
	std::function<void(uint8_t*, size_t)> OnDisconnect = NULL;
	std::function<void(uint8_t*, size_t)> OnUpdate = NULL;

	std::function<void(uint8_t*, size_t)> OnAccept = NULL;
	std::function<void(uint8_t*, size_t)> OnAdd = NULL;
	std::function<void(uint8_t*, size_t)> OnRemove = NULL;
	std::function<void(uint8_t*, size_t)> OnMessage = NULL;

	bool IsServer = false;

protected:
	int networkID = -1;

	uint8_t packetBuffer[PACKET_BUFFER_SIZE];

	//int maxPlayers = 50;
};