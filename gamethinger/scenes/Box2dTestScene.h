#pragma once
#include "../BaseScene.h"

#include <box2d/box2d.h>
#include <raylib.h>

class Box2dTestScene :public BaseScene
{
public:
	Box2dTestScene() //: gravity(0.0f, 10.0f), world(gravity)
	{
		SceneName = "Box2d Testing";
		gravity.x = 0.f;
		gravity.y = 10.f;
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = b2Vec2{ 0.0f, -10.0f };
		worldId = b2CreateWorld(&worldDef);
		world = &worldDef;
	}

	void Init();

	//void ClearScene();

	void Update(float deltaTime);
	void Render();
	void End();
};