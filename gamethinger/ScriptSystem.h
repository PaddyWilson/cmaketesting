#pragma once
#include "BaseSystem.h"
class ScriptSystem : public BaseSystem
{
public:
	ScriptSystem() { systemName = "ScriptSystem"; }
	void Init();
	void Update(float deltaTime);

private:
	void OnScriptAdd(entt::registry& reg, entt::entity entity);
	void OnScriptRemove(entt::registry& reg, entt::entity entity);
};

