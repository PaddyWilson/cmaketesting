#include "SceneManager.h"

SceneManager::~SceneManager()
{
	for(auto var : sceneList)
	{
		var.second->End();
		delete var.second; // delete all scenes
	}
}

void SceneManager::Update(float deltaTime)
{
	activeScene->Update(deltaTime);
}

void SceneManager::Render()
{
	activeScene->Render();
}

void SceneManager::RenderUI()
{
	activeScene->RenderUI();
}

void SceneManager::End()
{
	activeScene->End();
}

BaseScene* SceneManager::GetActiveScene()
{
	return activeScene;
}

int SceneManager::GetSceneCount()
{
	return sceneList.size();
}

BaseScene* SceneManager::AddScene(BaseScene* scene)
{
	entt::registry* registry = new entt::registry();
	entt::dispatcher* dispatcher = new entt::dispatcher();

	sceneList[scene->SceneName] = scene;
	activeScene = scene;
	activeScene->SetRegistry(registry);
	activeScene->SetDispatcher(dispatcher);
	activeScene->SetSystemManager(new SystemManager(registry, dispatcher));
	//activeScene->Init();
	return activeScene;

	//return AddScene(scene, registry, new SystemManager(registry));
}

//BaseScene* SceneManager::AddScene(BaseScene* scene, entt::registry* registry)
//{
//	return AddScene(scene, registry, new SystemManager(registry));
//}
//
//BaseScene* SceneManager::AddScene(BaseScene* scene, entt::registry* registry, SystemManager* systemManager)
//{
//	sceneList[scene->SceneName] = scene;
//	activeScene = scene;
//	activeScene->SetRegistry(registry);
//	activeScene->SetDispatcher(new entt::dispatcher);
//	activeScene->SetSystemManager(systemManager);
//	activeScene->Init();
//	return activeScene;
//}

BaseScene* SceneManager::ChangeScene(std::string sceneName)
{
	if (sceneList[sceneName] == NULL)
		return NULL;

	activeScene = sceneList[sceneName];
	return activeScene;
}

void SceneManager::RemoveScene(std::string sceneName)
{
	sceneList[sceneName]->End();
	delete sceneList[sceneName];
	sceneList[sceneName] = NULL;
}

std::vector<std::string> SceneManager::GetSceneNames()
{
	std::vector<std::string> names;
	for (auto var : sceneList)
		names.push_back(var.first);
	return names;
}
