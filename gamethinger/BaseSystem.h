#pragma once
#include <entt/entt.hpp>

#define DEFAULT_SCALING_FACTOR 10.0f

class BaseSystem
{	
protected:
	entt::registry* registry;
	entt::dispatcher* dispatcher;

public:
	std::string systemName = "Default Name";
	bool updateEnabled = true;
	bool renderEnabled = true;
	bool renderUIEnabled = true;

	virtual void Init() {}

	virtual void Update(float deltaTime) {}

	virtual void Render() {}
	virtual void RenderUI() {}

	virtual void Reset() {}
	virtual void End() {}

	void SetRegisty(entt::registry* registry) {
		this->registry = registry;
	}

	void SetDispatcher(entt::dispatcher* dispatcher) {
		this->dispatcher = dispatcher;
	}
};