#pragma once
#include "BaseScene.h"

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
		world = new b2World(gravity);
	}

	void Init();

	//void ClearScene();

	void Update(float deltaTime);
	void Render();
	void End();
};