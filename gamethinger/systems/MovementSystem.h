#pragma once
#include "../BaseSystem.h"
#include "../Components.h"

class MovementSystem : public BaseSystem
{
public:
	MovementSystem() { systemName = "MovementSystem"; }
	void Update(float deltaTime);
};

