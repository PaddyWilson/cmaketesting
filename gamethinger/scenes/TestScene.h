#pragma once
#include "../BaseScene.h"

#include "../Components.h"

class TestScene :  public BaseScene
{
public:
	TestScene() { SceneName = "Test Scene"; };
	~TestScene() {};
	void Init();
	void End();

	void Update(float deltaTime) ;
	void Render();
	//void RenderUI();
};
