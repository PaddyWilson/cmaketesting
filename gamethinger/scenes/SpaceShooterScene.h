#pragma once
#include "../BaseScene.h"
#include <raylib.h>

class SpaceShooterScene : public BaseScene
{
public:
	SpaceShooterScene();

	void Init();
	void End();

	void Update(float deltaTime);

private:
	entt::entity player;

	Sound shootSound;
	void Shoot();
};

