#include "SpaceShooterScene.h"

#include "PreFabs.h"
#include "Vector2Math.h"
#include <raymath.h>

#include "ScriptRegistry.h"

Velocity* playerV = NULL;

SpaceShooterScene::SpaceShooterScene()
{
	SceneName = "Space Shooter";
}

void SpaceShooterScene::Init()
{
	auto e = Prefabs::CreateCamera(registry,
		Vector2{ (float)Global::ScreenWidth / 2 , (float)Global::ScreenWidth / 2 },
		Vector2{ (float)Global::ScreenWidth / 2 , (float)Global::ScreenWidth / 2 });

	TextureManager::Load(Global::TexturePath + "kenney_simple-space\\PNG\\Retina\\ship_A.png", "shipA");
	TextureManager::Load(Global::TexturePath + "kenney_simple-space\\PNG\\Retina\\meteor_detailedLarge.png", "meteor_detailedLarge");
	TextureManager::Load(Global::TexturePath + "kenney_simple-space\\PNG\\Retina\\ship_B.png", "shipB");
	
	shootSound = LoadSound((Global::TexturePath + "kenney_space-shooter-redux\\Bonus\\sfx_laser1.ogg").c_str());

	std::vector<Vector2> vertsP;
	vertsP.push_back(Vector2{ 0.025f, -2.15f });
	vertsP.push_back(Vector2{ -2.95f, 0.825f });
	vertsP.push_back(Vector2{ 3.025f, 0.825f });
	vertsP.push_back(Vector2{ 1.675f, 2.25f });
	vertsP.push_back(Vector2{ -1.575f, 2.25f });

	auto entity = Prefabs::CreateSpriteCircle(registry, 70.f, 70.f, 12.8f, 12.8f, "shipA");
	registry->remove<BB_CIRCLE>(entity);
	Prefabs::AddPoly(registry, entity, vertsP, false);
	registry->get<Tag>(entity).tag = "PlayerShip";
	registry->get<SpriteComp>(entity).color = ORANGE;
	player = entity;
	//registry->get<ColorComp>(entity).color = ORANGE;
	playerV = &registry->replace<Velocity>(entity, 0.f, 0.f, 0.f);
	ScriptRegistry::AddScriptToEntity("SpaceShipController", registry, entity);

	entity = Prefabs::CreateBasicEntity(registry, Vector2(), "spawn script");
	ScriptRegistry::AddScriptToEntity("MeteorSpawnerScript", registry, entity);

	std::vector<Vector2> verts;
	verts.push_back(Vector2{ -3.1f, -2.95f });
	verts.push_back(Vector2{ 0.025f, -4.575f });
	verts.push_back(Vector2{ 3.1f, -2.95f });
	verts.push_back(Vector2{ 4.65f, 0.05f });
	verts.push_back(Vector2{ 3.1f, 3.1f });
	verts.push_back(Vector2{ 0.05f, 4.65f });
	verts.push_back(Vector2{ -3.1f, 3.1f });
	verts.push_back(Vector2{ -4.6f, 0.05f });

	entity = Prefabs::CreateSprite(registry, 40.f, 40.f, 12.8f, 12.8f, "meteor_detailedLarge");
	registry->remove<BB_RECTANGLE>(entity);
	Prefabs::AddPoly(registry, entity, verts, false);
	registry->get<Velocity>(entity).rotation = 12.f;
	registry->get<SpriteComp>(entity).color = BROWN;
	ScriptRegistry::AddScriptToEntity("MeteorController", registry, entity);

	entity = Prefabs::CreateSprite(registry, 60.f, 40.f, 12.8f, 12.8f, "meteor_detailedLarge");
	registry->remove<BB_RECTANGLE>(entity);
	Prefabs::AddPoly(registry, entity, verts, false);
	registry->get<Velocity>(entity).rotation = 280.f;
	registry->get<SpriteComp>(entity).color = BROWN;
	ScriptRegistry::AddScriptToEntity("MeteorController", registry, entity);

	entity = Prefabs::CreateSprite(registry, 80.f, 40.f, 12.8f, 12.8f, "meteor_detailedLarge");
	registry->remove<BB_RECTANGLE>(entity);
	Prefabs::AddPoly(registry, entity, verts, false);
	registry->get<Velocity>(entity).rotation = 20.f;
	registry->get<SpriteComp>(entity).color = BROWN;
	ScriptRegistry::AddScriptToEntity("MeteorController", registry, entity);

	entity = Prefabs::CreateSprite(registry, 10.f, 40.f, 12.8f, 12.8f, "meteor_detailedLarge");
	registry->remove<BB_RECTANGLE>(entity);
	Prefabs::AddPoly(registry, entity, verts, false);
	registry->get<Velocity>(entity).rotation = 320.f;
	registry->get<SpriteComp>(entity).color = BROWN;
	ScriptRegistry::AddScriptToEntity("MeteorController", registry, entity);

}
#include <imgui.h>
void SpaceShooterScene::Update(float deltaTime)
{
	Vector2 mouse = GetMousePosition() / 10;
	// is imgui being used
	ImGuiIO& io = ImGui::GetIO();
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !io.WantCaptureMouse)
	{
		std::vector<Vector2> verts;
		verts.push_back(Vector2{ -3.1f, -2.95f });
		verts.push_back(Vector2{ 0.025f, -4.575f });
		verts.push_back(Vector2{ 3.1f, -2.95f });
		verts.push_back(Vector2{ 4.65f, 0.05f });
		verts.push_back(Vector2{ 3.1f, 3.1f });
		verts.push_back(Vector2{ 0.05f, 4.65f });
		verts.push_back(Vector2{ -3.1f, 3.1f });
		verts.push_back(Vector2{ -4.6f, 0.05f });
		auto entity = Prefabs::CreateSprite(registry, mouse.x, mouse.y, 12.8f, 12.8f, "meteor_detailedLarge");
		registry->remove<BB_RECTANGLE>(entity);
		Prefabs::AddPoly(registry, entity, verts, false);
		registry->get<Velocity>(entity).rotation = 320.f;
		registry->get<SpriteComp>(entity).color = BROWN;
		ScriptRegistry::AddScriptToEntity("MeteorController", registry, entity);
	}

	BaseScene::Update(deltaTime);
}

void SpaceShooterScene::End()
{
	//do something?
}

void SpaceShooterScene::Shoot()
{
	auto& pos = registry->get<Position>(player);

	auto ship = Prefabs::CreateSpriteCircle(registry, 0, 0, 6.4, 6.4, "shipB");
	//registry->get<ColorComp>(ship).color = GREEN;
	auto& shipPos = registry->get<Position>(ship);
	auto& shipVel = registry->get<Velocity>(ship);

	//spawn a thing infront of the player
	auto deg = pos.rotation / (180 / 3.14159f);
	auto a = Vector2Rotate(Vector2(0.f, -2.f), deg);
	shipPos.position = pos.position + a;
	shipPos.rotation = pos.rotation;

	shipVel.velocity.y = -40;

	PlaySound(shootSound);
}
