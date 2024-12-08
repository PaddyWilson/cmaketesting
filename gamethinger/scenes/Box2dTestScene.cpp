#include "Box2dTestScene.h"
#include "../Components.h"
//
//#include <raylib.h>
#include "../PreFabs.h"
#include "../Vector2Math.h"
#pragma warning( disable: 4244)

void Box2dTestScene::Init()
{
	velocityIterations = 4;
	positionIterations = 2;

	auto e = Prefabs::CreateCamera(registry,
		Vector2{ (float)Global::ScreenWidth / 2 , (float)Global::ScreenWidth / 2 },
		Vector2{ (float)Global::ScreenWidth / 2 , (float)Global::ScreenWidth / 2 });

	auto entity = Prefabs::CreateBox2dRec(registry, worldId, 192 / 2, 100, 100, 1, Rigidbody2D::RigidbodyType::Static, PURPLE);
	//Prefabs::AddAABB(registry, entity, Vector2{ 100, 1 });

	entity = Prefabs::CreateBox2dRec(registry, worldId, 2, 50, 1, 50, Rigidbody2D::RigidbodyType::Static, PINK);
	//Prefabs::AddAABB(registry, entity, Vector2{ 1, 50 });

	entity = Prefabs::CreateBox2dRec(registry, worldId, 192, 100, 1, 25, Rigidbody2D::RigidbodyType::Static, GREEN);
	//Prefabs::AddAABB(registry, entity, Vector2{ 1, 25 });

	Rigidbody2D::RigidbodyType t = Rigidbody2D::RigidbodyType::Dynamic;
	int xR = 50;
	int yR = 40;
	int x = xR;
	int y = yR;
	for (size_t i = 0; i < 100; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			entity = Prefabs::CreateBox2dRec(registry, worldId, x, y, 1, 1, t, RED);
			//Prefabs::AddAABB(registry, entity, Vector2{ 1, 1 });

			entity = Prefabs::CreateBox2dCircle(registry, worldId, x + 2, y, 1, t, BLUE);
			//Prefabs::AddCircle(registry, entity, 1);
			x += 4;
		}
		x = xR;
		y -= 3;
	}
}

void Box2dTestScene::Update(float deltaTime)
{
	//simulate world
	b2World_Step(worldId, deltaTime, velocityIterations);
	//world->Step(deltaTime, velocityIterations, positionIterations);
	systemManager->Update(deltaTime);
}

void Box2dTestScene::Render()
{
	systemManager->Render();
}

void Box2dTestScene::End()
{

}
