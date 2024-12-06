#pragma once

#include <raylib.h>

#include "Script.h"
#include "Components.h"
#include "PreFabs.h"
#include "EventManager.h"

class TestController : public Script {
private:
	//Position* pos = nullptr;
public:
	TestController() {
		scriptName = "TestController";
	}

	void Init() {
		//pos = &reg->get<Position>(entity);
	}

	void Update(float dt) {
		Position* pos = &reg->get<Position>(entity);
		if (IsKeyDown(KEY_A))
			pos->position.x -= 100 * dt;
		if (IsKeyDown(KEY_D))
			pos->position.x += 100 * dt;
		if (IsKeyDown(KEY_W))
			pos->position.y -= 100 * dt;
		if (IsKeyDown(KEY_S))
			pos->position.y += 100 * dt;
	}
};

class TestController2 : public Script {
private:
	//Position* pos = nullptr;
public:
	TestController2() {
		scriptName = "TestController2";
	}

	void Init() {
		//pos = &reg->get<Position>(entity);
	}

	void Update(float dt) {
		Position* pos = &reg->get<Position>(entity);

		if (IsKeyDown(KEY_A))
			pos->position.x -= 50 * dt;
		if (IsKeyDown(KEY_D))
			pos->position.x += 50 * dt;
		if (IsKeyDown(KEY_W))
			pos->position.y -= 50 * dt;
		if (IsKeyDown(KEY_S))
			pos->position.y += 50 * dt;
	}
};

struct IsRocketFlag {};

class SpaceShipController : public Script {
private:
	//Position* pos = nullptr;
	//Velocity* vel = nullptr;
public:
	float speed = 20.0f;
	float rotation = 200.f;

	Sound shootSound;

	SpaceShipController() {
		scriptName = "SpaceShipController";
		shootSound = LoadSound((Global::TexturePath + "kenney_space-shooter-redux\\Bonus\\sfx_laser1.ogg").c_str());
	}

	void Init() {
		//pos = &reg->get<Position>(entity);
		//vel = &reg->get<Velocity>(entity);
	}

	void Update(float dt) {

		Position* pos = &reg->get<Position>(entity);
		Velocity* vel = &reg->get<Velocity>(entity);

		if (IsKeyDown(KEY_W))
			vel->velocity.y = -speed;// *dt;
		else if (IsKeyDown(KEY_S))
			vel->velocity.y = speed;// *dt;
		else
			vel->velocity.y = 0;

		if (IsKeyDown(KEY_A))
			vel->rotation = -rotation;// *dt;
		else if (IsKeyDown(KEY_D))
			vel->rotation = rotation;// *dt;
		else
			vel->rotation = 0;

		if (IsKeyPressed(KEY_SPACE))
			Shoot();
	}

	void Shoot()
	{
		std::vector<Vector2> verts;
		verts.push_back(Vector2{ 0.0f, -1.5f });
		verts.push_back(Vector2{ 0.75f, 0.8f });
		verts.push_back(Vector2{ 0.0f, 1.5f });
		verts.push_back(Vector2{ -0.7f, 0.8f });

		auto ship = Prefabs::CreateSpriteCircle(reg, 0, 0, 6.4, 6.4, "shipB");
		//auto* bb = &reg->get<BB_CIRCLE>(ship);
		//bb->isStatic = false;
		reg->remove<BB_CIRCLE>(ship);
		auto* poly = Prefabs::AddPoly(reg, ship, verts, false);

		reg->get<Tag>(ship).tag = "rocket";
		reg->get<SpriteComp>(ship).color = GREEN;
		auto& rocketPos = reg->get<Position>(ship);
		auto& rocketVel = reg->get<Velocity>(ship);
		reg->emplace<IsRocketFlag>(ship);

		Position* pos = &reg->get<Position>(entity);
		//spawn a thing infront of the player
		auto deg = pos->rotation / (180 / 3.14159f);
		auto spawnOffset = Vector2Rotate(Vector2(0.f, -2.f), deg);
		rocketPos.position = pos->position + spawnOffset;
		rocketPos.rotation = pos->rotation;

		rocketVel.velocity.y = -40;

		PlaySound(shootSound);

		poly->Move(rocketPos.position, rocketPos.offset, rocketPos.rotation);
	}
};



class MeteorController : public Script {
private:
	//Position* pos = nullptr;
	//BB_POLY* bb = nullptr;
public:
	MeteorController() {
		scriptName = "MeteorController";
	}

	~MeteorController() {}

	void Init() {
		//pos = &reg->get<Position>(entity);
		//bb = &reg->get<BB_POLY>(entity);

		dis->sink<CollisionPair>().connect<&MeteorController::OnCollision>(this);
	}

	void Remove()
	{
		if (dis != nullptr)
			dis->sink<CollisionPair>().disconnect<&MeteorController::OnCollision>(this);
	}

	void Update(float dt) {	}

	void OnCollision(CollisionPair& pair)
	{
		if (pair.entity1 == entity) {
			OnCollisionMatch(pair.entity1, pair.entity2);
		}
		else if (pair.entity2 == entity) {
			OnCollisionMatch(pair.entity2, pair.entity1);
		}
	}

	void OnCollisionMatch(entt::entity e1, entt::entity e2)
	{
		auto tag = reg->try_get<Tag>(e2)->tag;
		if (tag == "rocket")
		{
			reg->emplace_or_replace<DestroyFlag>(entity);
			reg->emplace_or_replace<DestroyFlag>(e2);
		}
	}
};

class MeteorSpawnerScript : public Script {
private:
	//Position* pos = nullptr;
	//BB_POLY* bb = nullptr;
public:
	float spawnTime = .02;
	float countDown;

	MeteorSpawnerScript() {
		scriptName = "MeteorSpawnerScript";
		countDown = spawnTime;
	}

	~MeteorSpawnerScript() {}

	void Init() {
		//dis->sink<CollisionPair>().connect<&MeteorController::OnCollision>(this);
	}

	void Remove() {	}

	void Update(float dt) {
		countDown -= dt;

		if (countDown < 0) {
			std::vector<Vector2> verts;
			verts.push_back(Vector2{ -3.1f, -2.95f });
			verts.push_back(Vector2{ 0.025f, -4.575f });
			verts.push_back(Vector2{ 3.1f, -2.95f });
			verts.push_back(Vector2{ 4.65f, 0.05f });
			verts.push_back(Vector2{ 3.1f, 3.1f });
			verts.push_back(Vector2{ 0.05f, 4.65f });
			verts.push_back(Vector2{ -3.1f, 3.1f });
			verts.push_back(Vector2{ -4.6f, 0.05f });

			int x = rand() % GetScreenWidth();
			int y = rand() % GetScreenHeight();

			int r = rand() % 640;
			r -= 320;

			auto entity = Prefabs::CreateSprite(reg, (float)(x / 10), (float)(y / 10), 12.8f, 12.8f, "meteor_detailedLarge");
			reg->remove<BB_RECTANGLE>(entity);
			Prefabs::AddPoly(reg, entity, verts, false);
			reg->get<Velocity>(entity).rotation = (float)r;
			reg->get<SpriteComp>(entity).color = BROWN;
			ScriptRegistry::AddScriptToEntity("MeteorController", reg, entity);

			countDown = spawnTime;
		}
	}
};