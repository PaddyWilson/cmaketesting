#pragma once
#include "../BaseSystem.h"
class Camera2DSystem : public BaseSystem
{
public:	
	Camera2DSystem()
	{
		systemName = "Camera2DSystem";
	}
	//void Init() {}

	//void PreUpdate(float dt) {}
	void Update(float deltaTime);
	//void PostUpdate(float dt) {}

	//void Render() {}
	//void RenderUI() {}

	//void End() {}
};

