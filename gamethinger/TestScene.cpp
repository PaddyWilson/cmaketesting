#include "TestScene.h"

#include "PreFabs.h"

int clickI = 0;
void ClickTest() {
	std::cout << "Clicked " << clickI++ << std::endl;
}

void TestScene::Init()
{
	auto e = Prefabs::CreateCamera(registry,
		Vector2{ (float)Global::ScreenWidth / 2 , (float)Global::ScreenWidth / 2 },
		Vector2{ (float)Global::ScreenWidth / 2 , (float)Global::ScreenWidth / 2 });

	//create entitys
	auto entity = Prefabs::CreateSprite(registry, 10, 10, 10, 15, "");
	registry->get<Velocity>(entity).rotation = 2.f;
	//std::get<Velocity>(comps).rotation = 20.f;	

	entity = Prefabs::CreateSprite(registry, 20, 40, 10, 15, "");
	
	registry->get<Position>(entity).rotation = 90;
	registry->get<Velocity>(entity).rotation = 20.f;
	registry->emplace<Clickable>(entity, Clickable::Action::PRESSED, &ClickTest);

	entity = Prefabs::CreateRectangle(registry, 40.f, 40.f, 10.f, 10.f, GREEN);
	registry->get<Velocity>(entity).rotation = 20.f;
	registry->emplace<Clickable>(entity, Clickable::Action::PRESSED, &ClickTest);

	entity = Prefabs::CreateCircle(registry, 60.f, 40.f, 4.5, BLUE);
	registry->get<Velocity>(entity).rotation = 40.f;
	registry->emplace<Clickable>(entity, Clickable::Action::PRESSED, &ClickTest);
	registry->emplace<LineComp>(entity, 0.f, 0.f, 10.f, 10.f, 1.0f);//, RED);

	//parent test	
	entt::entity parent = Prefabs::CreateRectangle(registry, 150.f, 40.f, 10.f, 10.f, BLUE);
	registry->replace<Velocity>(parent, 0.f, 0.f, 30.f);
	registry->emplace<LineComp>(parent, -20.f, 20.f, 20.f, -20.f, 1.f);// , GREEN);
	registry->emplace<Clickable>(parent, Clickable::Action::PRESSED, &ClickTest);

	//entt::entity parent = entity;
	for (size_t i = 0; i < 10; i++)
	{
		if (i % 2 == 0)
			entity = Prefabs::CreateCircle(registry, 80, 40, 4.5, RED);
		else
			entity = Prefabs::CreateRectangle(registry, 80.f, 40.f, 10.f, 10.f, GREEN);
		auto p = registry->emplace<Parent>(entity, parent);
		//registry->remove<Renderable>(entity);
		registry->emplace<Clickable>(entity, Clickable::Action::UP, &ClickTest);//cant click on line yet
	}

	//child
	//box
	entity = Prefabs::CreateRectangle(registry, 15.f, 15.f, 10.f, 10.f, RED);
	//registry->replace<Velocity>(entity, 0.f, 0.f, 60.f);
	registry->emplace<Parent>(entity, parent);
	registry->emplace<Clickable>(entity, Clickable::Action::UP, &ClickTest);

	//circle
	entity = Prefabs::CreateCircle(registry, -10.f, -10.f, 4.5, PINK);
	//registry->replace<Velocity>(entity, 0.f, 0.f, 70.f);
	registry->emplace<Parent>(entity, parent);
	registry->emplace<Clickable>(entity, Clickable::Action::UP, &ClickTest);

	//line
	entity = Prefabs::CreateLine(registry, -15.f, -15.f, 10.f, 10.f, PURPLE);
	registry->emplace<Parent>(entity, parent);
	registry->emplace<Clickable>(entity, Clickable::Action::UP, &ClickTest);//cant click on line yet

	//button
	entity = Prefabs::CreateButton(registry, "Button", 80, 80, 5, &ClickTest, RED);
	registry->emplace<Velocity>(entity, 0.f, 0.f, 70.f);
	//registry->emplace<Parent>(entity, parent);


	parent = Prefabs::CreateRectangle(registry, 175.f, 40.f, 1.f, 1.f, PINK);
	registry->replace<Velocity>(parent, 0.f, 0.f, 30.f);

	////box
	//entity = Prefabs::CreateRectangle(registry, 1.f, 1.f, 1.f, 1.f, LIME);
	//registry->replace<Velocity>(entity, 0.f, 0.f, 60.f);
	//registry->emplace<Parent>(entity, parent);
	//parent = entity;

	for (size_t i = 0; i < 100; i++)
	{
		//box
		entity = Prefabs::CreateRectangle(registry, 1.f, 1.f, 1.f, 1.f, LIGHTGRAY);
		registry->replace<Velocity>(entity, 0.f, 0.f, 0.f);
		registry->emplace<Parent>(entity, parent);
		parent = entity;
	}
}

void TestScene::Update(float deltaTime)
{
	systemManager->Update(deltaTime);
}

void TestScene::Render()
{
	systemManager->Render();

	DrawLine(0, 400, 10000, 400, PURPLE);
	DrawLine(200, -2400, 200, 4000, PURPLE);
	DrawLine(400, -1400, 400, 4000, PURPLE);
	DrawLine(600, -1400, 600, 4000, PURPLE);
	DrawLine(800, -1400, 800, 4000, PURPLE);
	DrawLine(1500, -1400, 1500, 4000, PURPLE);
}

//void TestScene::RenderUI()
//{
//	systemManager->RenderUI();
//}

void TestScene::End()
{
	//undo the things
}
