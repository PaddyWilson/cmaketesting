#pragma once
#include <map>
#include <vector>

#include "BaseScene.h"

class SceneManager
{
private:
	std::map<std::string, BaseScene*> sceneList;
	BaseScene* activeScene = nullptr;

public:
	~SceneManager();

	void Update(float deltaTime);
	void Render();
	void RenderUI();

	void End();

	BaseScene* GetActiveScene();
	int GetSceneCount();

	BaseScene* AddScene(BaseScene* scene);
	//BaseScene* AddScene(BaseScene* scene, entt::registry* registry);
	//BaseScene* AddScene(BaseScene* scene, entt::registry* registry, SystemManager* systemManager);

	BaseScene* ChangeScene(std::string sceneName);
	void RemoveScene(std::string sceneName);

	std::vector<std::string> GetSceneNames();
};

