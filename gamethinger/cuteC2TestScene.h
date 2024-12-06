#pragma once
#include "BaseScene.h"
#include "Components.h"

class Cute_c2TestScene : public BaseScene
{
private:
	Position* pos;

public:
	Cute_c2TestScene() { SceneName = "Cute C2 collision"; };
	void Init();
	void End();

	void Update(float deltaTime);
	void Render();
	void RenderUI();
};

