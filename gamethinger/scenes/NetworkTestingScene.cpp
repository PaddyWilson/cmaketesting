#include "NetworkTestingScene.h"

#include "../PreFabs.h"

NetworkTestingScene::NetworkTestingScene()
{
	SceneName = "Network Testing";
}

NetworkTestingScene::~NetworkTestingScene()
{
}

void NetworkTestingScene::Init()
{
	//SetWindowSize(400, 400);
	auto e = Prefabs::CreateCamera(registry,
		Vector2{ (float)Global::ScreenWidth / 2 , (float)Global::ScreenWidth / 2 },
		Vector2{ (float)Global::ScreenWidth / 2 , (float)Global::ScreenWidth / 2 });
	//create buttons
	connectButton = Prefabs::CreateButton(registry, "Connect", 10, 10, 2, [&] {Connect(); }, RED, BLACK);
	//connectComp = registry->get<Clickable>(connectButton);

	disconnectButton = Prefabs::CreateButton(registry, "Disconnect", 10, 15, 2, [&] {Disconnect(); }, RED, BLACK);
	//disconnectComp = registry->get<Clickable>(disconnectButton);

	sendMessageButton = Prefabs::CreateButton(registry, "Send Message", 10, 20, 2, [&] {SendMessage("Send Message clicked"); }, RED, BLACK);
	sendUpdateButton = Prefabs::CreateButton(registry, "Send Update", 10, 25, 2, [&] {SendUpdate("Send Update clicked"); }, RED, BLACK);
	//sendComp = registry->get<Clickable>(sendMessageButton);

	statusLabel = Prefabs::CreateLabel(registry, "Disconnected", 10, 30, 2, BLACK);
	//label = registry->get<TextColor>(statusLabel);

	//create client/server
	client = new NetworkClient();
	server = new NetworkServer();

	//can use this or
	server->OnUpdate = std::bind(&NetworkTestingScene::testFunc, this, std::placeholders::_1, std::placeholders::_2);
	//this to bind a class method to a function pointer. I like the above more
	/* server->OnUpdate =[&](uint8_t*data, size_t offset)
	{
		int id = ReadInt(data, &offset);
		std::string msg = ReadString(data, &offset);
		std::cout << "clientID:" << id << " Size:" << msg.size() << " MSG:" << msg << std::endl;
	};*/

	//start server
	server->Connect(NETWORK_IP, NETWORK_PORT);

	UpdateIU();
}

void NetworkTestingScene::End()
{
}

void NetworkTestingScene::Update(float deltaTime)
{
	systemManager->Update(deltaTime);

	if (server->Connected())
		server->Update(deltaTime);

	client->Update(deltaTime);

	UpdateIU();
}

void NetworkTestingScene::Render()
{
	systemManager->Render();
}

void NetworkTestingScene::testFunc(uint8_t* data, size_t offset)
{
	int id = ReadInt(data, &offset);
	std::string msg = ReadString(data, &offset);
	std::cout << "clientID:" << id << " Size:" << msg.size() << " MSG:" << msg << std::endl;
}

void NetworkTestingScene::Connect()
{
	client->Connect(NETWORK_IP, NETWORK_PORT);
}

void NetworkTestingScene::Disconnect()
{
	client->Disconnect();
}

void NetworkTestingScene::SendMessage(std::string string)
{
	client->Send(string);
}

void NetworkTestingScene::SendUpdate(std::string string)
{
	if (!client->Connected())
		return;

	uint8_t data[4000];

	size_t offset = 0;

	PackString(data, &offset, string);

	/*int number = string.size();

	uint8_t watch[4] = {
		number & 255,
		(number >> 8) & 255,
		(number >> 16) & 255,
		(number >> 24) & 255 };

	data[offset + 0] = watch[0];
	data[offset + 1] = watch[1];
	data[offset + 2] = watch[2];
	data[offset + 3] = watch[3];
	offset += 4;

	std::copy(string.begin(), string.end(), &data[offset]);
	offset += string.size();*/

	client->Send(data, (int)offset);
}

void NetworkTestingScene::UpdateIU()
{
	if (client->Connected())
	{
		registry->get<Clickable>(connectButton).enabled = false;
		registry->get<Clickable>(disconnectButton).enabled = true;
		registry->get<Clickable>(sendMessageButton).enabled = true;
		registry->get<TextComp>(statusLabel).text = "Connected";
	}
	else
	{
		registry->get<Clickable>(connectButton).enabled = !false;
		registry->get<Clickable>(disconnectButton).enabled = !true;
		registry->get<Clickable>(sendMessageButton).enabled = !true;
		registry->get<TextComp>(statusLabel).text = "Disconnected";
	}
}
