#pragma once
#include "../BaseScene.h"

#include "../network/NetworkBase.h"
#include "../network/NetworkClient.h"
#include "../network/NetworkServer.h"

#include "../Components.h"

class NetworkTestingScene :  public BaseScene
{
private:
	entt::entity connectButton;
	//Clickable& connectComp;

	entt::entity disconnectButton;
	//Clickable& disconnectComp;
	
	entt::entity statusLabel;
	//TextColor& label;

	entt::entity sendMessageButton;
	entt::entity sendUpdateButton;
	//Clickable& sendComp;

	NetworkBase* client;
	NetworkBase* server;

public:
	NetworkTestingScene();
	~NetworkTestingScene();
	void Init();
	void End();

	void Update(float deltaTime);
	void Render();
	//void RenderUI();

	void testFunc(uint8_t* data, size_t offset);

private:
	void UpdateIU();

	void Connect();
	void Disconnect();
	void SendMessage(std::string string);
	void SendUpdate(std::string string);
};