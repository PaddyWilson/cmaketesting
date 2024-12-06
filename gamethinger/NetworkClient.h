#pragma once

#include "NetworkBase.h"

#include <iostream>
#include <vector>

//using namespace std;

class NetworkClient : public NetworkBase
{
public:
	void Connect(std::string ipAddress, int port);
	void Disconnect();
	void Update(float dt);

	bool Connected(); 
	int GetNetworkID();

	void Send(std::string message);
	void SendToAll(std::string message);

	void Send(uint8_t data[], int dataSize);
	void SendTo(uint8_t data[], int dataSize, int networkID);
	void SendToAll(uint8_t data[], int dataSize);
	void SendToAll(uint8_t data[], int dataSize, int exclude);

	bool IsServer = false;
};

