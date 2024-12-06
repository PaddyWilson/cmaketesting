#pragma once
#include <entt/entt.hpp>
#include <box2d/box2d.h>

#include "Serialize.h"
#include "SystemManager.h"

class BaseScene
{
private:
	Serialize serializer;

protected:
	//maybe move to Physics System?
	b2Vec2 gravity{ 0.0f, 10.f };
	b2WorldDef* world;
	b2WorldId worldId;
	float timeStep = 1.0f / 60.0f;
	int velocityIterations = 6;
	int positionIterations = 2;

	entt::registry* registry = nullptr;
	SystemManager* systemManager = nullptr;
	entt::dispatcher* dispatcher = nullptr;
public:
	friend class Serialize;
	std::string SceneName = "Default Scene Name";

	BaseScene() {
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = (b2Vec2){0.0f, -10.0f};
		worldId = b2CreateWorld(&worldDef);
		world = &worldDef;
	};

	virtual void Init() {};
	virtual void End() {};
	virtual void ClearScene();

	virtual void Update(float deltaTime);

	virtual void Render() { systemManager->Render(); };
	virtual void RenderUI() { systemManager->RenderUI(); };
	entt::entity CreateEntity();

	void Save(std::string filePath);
	void Load(std::string filePath);

	template<typename T>
	void RegisterComponent(std::string compName, int priority = 5) {
		serializer.RegisterComponent<T>(compName, priority);
	}

	void SetRegistry(entt::registry* registry) { this->registry = registry; }
	entt::registry* GetRegistry() { return registry; };
	void SetDispatcher(entt::dispatcher* dispatcher) { this->dispatcher = dispatcher; }
	entt::dispatcher* GetDispatcher() { return dispatcher; };
	void SetSystemManager(SystemManager* systemManager) { this->systemManager = systemManager; }
	SystemManager* GetSystemManager() { return systemManager; }

	b2WorldDef* GetWorld() { return world; }

//private:
//	vector<ImguiWindow*> imguiWindows;
//public:
//	virtual void ImGuiInit() {
//		rlImGuiSetup(true);//imgui
//	};
//
//	virtual void ImGuiRender() {
//		rlImGuiBegin();//imgui start
//
//		//menu bar
//		ImguiMenu::DrawMenu();
//
//		for (size_t i = 0; i < imguiWindows.size(); i++)
//			imguiWindows[i]->Draw();
//
//		rlImGuiEnd();//imgui end
//	};
};