#include "ScriptSystem.h"

#include "../Components.h"

void ScriptSystem::Init()
{
	//registry->on_construct<ScriptComp>().connect<&ScriptSystem::OnScriptAdd>(this);
	registry->on_destroy<ScriptComp>().connect<&ScriptSystem::OnScriptRemove>(this);
}

#include <iostream>
void ScriptSystem::Update(float deltaTime)
{
	auto view = registry->view<ScriptComp>();
	//std::cout << view.size() << std::endl;
	for (auto [entity, script] : view.each()) {

		if (script.scriptInstance == nullptr && script.InstantiateScript != nullptr)
		{
			script.scriptInstance = script.InstantiateScript();

			if (script.scriptInstance == nullptr)
				return;

			script.scriptInstance->entity = entity;
			script.scriptInstance->reg = registry;
			script.scriptInstance->dis = dispatcher;

			script.scriptInstance->Init();
		}

		if (script.scriptInstance != nullptr)
			script.scriptInstance->Update(deltaTime);
	}
}

void ScriptSystem::OnScriptAdd(entt::registry& reg, entt::entity entity)
{
	auto scripComp = reg.get<ScriptComp>(entity);

	scripComp.InstantiateScript();

	scripComp.scriptInstance->entity = entity;
	scripComp.scriptInstance->reg = &reg;

	scripComp.scriptInstance->Init();
}

void ScriptSystem::OnScriptRemove(entt::registry& reg, entt::entity entity)
{
	auto scripComp = reg.get<ScriptComp>(entity);
	if (scripComp.scriptInstance != nullptr) 
	{
		scripComp.scriptInstance->Remove();
		scripComp.DestroyScript(&scripComp);
	}
}
