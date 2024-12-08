#pragma once
#include "BaseSystem.h"

#include "network/NetworkBase.h"

class NetworkSystem : public BaseSystem
{
public:
	NetworkSystem() { systemName = "NetworkSystem"; }
	void Init() {}
	void Update(float deltaTime);
	void Render() {}
	void RenderUI() {}
	void End() {}
};

