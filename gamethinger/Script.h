#pragma once

#include <entt/entt.hpp>

class Script {
public:
	std::string scriptName = "DefaultName";

	entt::entity entity;
	entt::registry* reg;
	entt::dispatcher* dis;

	virtual void Init() {};
	virtual void Remove() {};
	virtual void Update(float deltaTime) {};
	virtual void Render() {};
};