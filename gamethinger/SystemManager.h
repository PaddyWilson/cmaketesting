#pragma once
#include <entt/entt.hpp>
#include <vector>
#include "BaseSystem.h"

class SystemManager
{
private:
	entt::registry* registry;
	entt::dispatcher* dispatcher;
	std::vector<BaseSystem*> systems;

public:
	SystemManager(entt::registry* registry, entt::dispatcher* dispatcher)
	{
		this->registry = registry;
		this->dispatcher = dispatcher;
	}

	void Init()
	{
		for (size_t i = 0; i < systems.size(); i++)
			systems[i]->Init();
	}

	void ResetSystems()
	{
		for (size_t i = 0; i < systems.size(); i++)
			systems[i]->Reset();
	}

	void Update(float deltaTime)
	{
		for (size_t i = 0; i < systems.size(); i++)
			if (systems[i]->updateEnabled)
				systems[i]->Update(deltaTime);
	}

	void Render()
	{
		for (size_t i = 0; i < systems.size(); i++)
			if (systems[i]->renderEnabled)
				systems[i]->Render();
	}

	void RenderUI()
	{
		for (size_t i = 0; i < systems.size(); i++)
			if (systems[i]->renderUIEnabled)
				systems[i]->RenderUI();
	}

	void AddSystem(BaseSystem* system)
	{
		systems.push_back(system);
		system->SetRegisty(registry);
		system->SetDispatcher(dispatcher);
		system->Init();
	}

	friend class ImguiSystemsWindow;
};

