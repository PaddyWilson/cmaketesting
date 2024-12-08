#include "MTGGameScene.h"

#include "../PreFabs.h"
#include "../Global.h"

#pragma warning( disable: 4244)

void OnUpdateTest(entt::registry& reg, entt::entity entity) {
	auto& pos = reg.get<Position>(entity);

	cout << pos.position.x << endl;
}

int iasdfiasodf = 0;

void MTGGameScene::Init()
{
	auto e = Prefabs::CreateCamera(registry,
		Vector2{ (float)Global::ScreenWidth / 2 , (float)Global::ScreenWidth / 2 },
		Vector2{ (float)Global::ScreenWidth / 2 , (float)Global::ScreenWidth / 2 });

	registry->emplace<Velocity>(e, Vector2{ 10.0f,0.0f }, 0.0f);

	//should have buttons for life/grave/exile for each player 

	//Create Buttons
	float y = 5;
	float x = 10;
	float space = 2.5;
	float size = 2.0;

	Prefabs::CreateButtonUI(registry, "Play", x, y, size, [&] {PlayCard(); }, BLANK, RED);				y += space;
	Prefabs::CreateButtonUI(registry, "Draw", x, y, size, [&] {DrawCard(); }, BLANK, BLACK);				y += space;
	Prefabs::CreateButtonUI(registry, "Return", x, y, size, [&] {ReturnCard(); }, BLANK, BLACK);			y += space;
	Prefabs::CreateButtonUI(registry, "Tap", x, y, size, [&] {TapCard(); }, PURPLE, BLACK);				y += space;
	Prefabs::CreateButtonUI(registry, "Untap All", x, y, size, [&] {UntapAll(); }, PURPLE, BLACK);		y += space;
	Prefabs::CreateButtonUI(registry, "OnTop", x, y, size, [&] {OnTop(); }, PURPLE, BLACK);				y += space;
	Prefabs::CreateButtonUI(registry, "OnBottom", x, y, size, [&] {OnBottom(); }, PURPLE, BLACK);			y += space;
	Prefabs::CreateButtonUI(registry, "Shuffle", x, y, size, [&] {Shuffle(); }, PURPLE, BLACK);			y += space;
	Prefabs::CreateButtonUI(registry, "Flip", x, y, size, [&] {FlipOver(); }, PURPLE, BLACK);				y += space;
	Prefabs::CreateButtonUI(registry, "NextToken", x, y, size, [&] {NextToken(); }, PURPLE, BLACK);		y += space;
	Prefabs::CreateButtonUI(registry, "Deck Search", x, y, size, [&] {ScearchDeck(); }, PURPLE, BLACK);	y += space;
	Prefabs::CreateButtonUI(registry, "To Grave", x, y, size, [&] {ToGrave(); }, PURPLE, BLACK);			y += space;
	Prefabs::CreateButtonUI(registry, "To Exile", x, y, size, [&] {ToExile(); }, PURPLE, BLACK);			y += space;
	Prefabs::CreateButtonUI(registry, "View Grave", x, y, size, [&] {ViewGrave(); }, PURPLE, BLACK);		y += space;
	Prefabs::CreateButtonUI(registry, "View Exile", x, y, size, [&] {ViewExile(); }, PURPLE, BLACK);		y += space;


	//only runs when registry->patch<Position>(entity); is used on the entity
	//https://github.com/skypjack/entt/blob/master/docs/md/entity.md#observe-changes
	registry->on_update<Position>().connect<&OnUpdateTest>();

	string dir = (string)GetWorkingDirectory() + "\\Decks\\ZadaHedronGrinder.json";

	if (!FileExists(dir.c_str())) {
		cout << "No deck file" << endl;
		return;
	}

	std::string deckJson = LoadFileText(dir.c_str());
	cout << "Loading Deck" << endl;
	deck = ScryfallJson::CreateDeck(deckJson);
	cout << "Downloading Deck" << endl;
	ScryfallJson::DownloadImages(deckJson);
	cout << "Loading Textures" << endl;
	ScryfallJson::LoadTextures(deckJson);

	float xi = 5, yi = 50;

	for (size_t i = 0; i < deck.cards.size(); i++)
	{
		MTGCard& card = deck.cards[i];
		entt::entity e = Prefabs::CreateSprite(registry, xi, yi, 10, 14, card.front);
		xi += 11;

		registry->emplace<Clickable>(e, Clickable::Action::PRESSED, [&]() { cout << "Card click " << iasdfiasodf++ << endl; });
		registry->emplace<MTGCard>(e, card.name, card.type, card.id, card.cardType, card.location, card.counters, card.owner, card.fliped, card.front, card.back);
	}
}

void MTGGameScene::End()
{

}

void MTGGameScene::Update(float deltaTime)
{
	systemManager->Update(deltaTime);
}

void MTGGameScene::Render()
{
	systemManager->Render();
}

void MTGGameScene::RenderUI()
{
	systemManager->RenderUI();
}


void MTGGameScene::PlayCard()
{
	std::cout << "PlayCard " << iasdfiasodf++ << std::endl;
}

void MTGGameScene::DrawCard()
{
	cout << "DrawCard " << iasdfiasodf++ << endl;
}

void MTGGameScene::ReturnCard()
{
	cout << "ReturnCard " << iasdfiasodf++ << endl;
}

void MTGGameScene::TapCard()
{
	cout << "TapCard " << iasdfiasodf++ << endl;
}

void MTGGameScene::UntapAll()
{
	cout << "UntapAll " << iasdfiasodf++ << endl;
}

void MTGGameScene::OnTop()
{
	std::cout << "OnTop " << iasdfiasodf++ << std::endl;
}

void MTGGameScene::OnBottom()
{
	std::cout << "OnBottom " << iasdfiasodf++ << std::endl;
}

void MTGGameScene::Shuffle()
{
	std::cout << "Shuffle " << iasdfiasodf++ << std::endl;
}

void MTGGameScene::FlipOver()
{
	std::cout << "FlipOver " << iasdfiasodf++ << std::endl;
}

void MTGGameScene::NextToken()
{
	std::cout << "NextToken " << iasdfiasodf++ << std::endl;
}

void MTGGameScene::ScearchDeck()
{
	std::cout << "ScearchDeck " << iasdfiasodf++ << std::endl;
}

void MTGGameScene::ToGrave()
{
	std::cout << "ToGrave " << iasdfiasodf++ << std::endl;
}

void MTGGameScene::ToExile()
{
	std::cout << "ToExile " << iasdfiasodf++ << std::endl;
}

void MTGGameScene::ViewGrave()
{
	std::cout << "ViewGrave " << iasdfiasodf++ << std::endl;
}

void MTGGameScene::ViewExile()
{
	std::cout << "ViewExile " << iasdfiasodf++ << std::endl;
}
