#pragma once
#include "../BaseSystem.h"

class ParentChildSystem : public BaseSystem
{
public:
	ParentChildSystem()	{systemName = "ParentChildSystem";}
	void Init();
	void Update(float deltaTime);

private:
	void AddParent(entt::registry& reg, entt::entity entity);
	void RemoveParent(entt::registry& reg, entt::entity entity);

	void UpdateParentDepthOfChildren(entt::registry& reg, entt::entity entity, int depth);

	void OnSerializeLoad(entt::registry* reg);
};

