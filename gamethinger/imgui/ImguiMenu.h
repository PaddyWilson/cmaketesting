#pragma once

#include <vector>
#include <functional>

#include "ImguiWindow.h"
#include "../SceneManager.h"

class ImguiMenuItemBase
{
public:
	std::string label = "Default";
	virtual void Run() = 0;
};

class ImguiMenuItemFunction : public ImguiMenuItemBase
{
public:
	std::function<void()> function = NULL;
	void Run();
};

class ImguiMenuItemToggle : public ImguiMenuItemBase
{
public:
	bool* function = NULL;
	void Run();
};

namespace ImguiMenu
{
	static std::vector<ImguiMenuItemBase*> fileMenu;
	static std::vector<ImguiMenuItemBase*> windowMenu;

	static std::vector<ImguiWindow*> customWindows;

	//can't assign with "=" operator, use SetSceneManager()
	static SceneManager* ImguiMenuSceneManager;
	void SetSceneManager(SceneManager* sceneManager);

	void AddFileMenu(std::string label, std::function<void()> function);
	void AddFileMenu(std::string label, bool *b);

	void AddWindowMenu(std::string label, std::function<void()> function);
	void AddWindowMenu(std::string label, bool* b);

	void AddCustomWindow(ImguiWindow* window);

	void DrawMenu();
};





