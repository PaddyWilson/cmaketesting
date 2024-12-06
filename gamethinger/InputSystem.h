#pragma once
#include "BaseSystem.h"
#include "Components.h"

class InputSystem : public BaseSystem
{
public:
	InputSystem() { systemName = "InputSystem"; }
	void Update(float deltaTime);
};

