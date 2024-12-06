#pragma once
#include "BaseSystem.h"
class HoverSystem :  public BaseSystem
{
public:
	HoverSystem() { systemName = "HoverSystem"; }
	//void Init();

	//void PreUpdate(float dt)
	void Update(float deltaTime);
	//void PostUpdate(float dt)

	//void Render() 
	//void RenderUI()

	//void End()
};

