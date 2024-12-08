#include "ImguiMenu.h"

//SceneManager* ImguiMenu::ImguiMenuSceneManager = NULL;

void ImguiMenu::SetSceneManager(SceneManager* sceneManager)
{
	ImguiMenuSceneManager = sceneManager;
}

void ImguiMenu::AddFileMenu(std::string label, std::function<void()> function)
{
	ImguiMenuItemFunction* item = new ImguiMenuItemFunction();
	item->label = label;
	item->function = function;
	fileMenu.push_back(item);
}

void ImguiMenu::AddFileMenu(std::string label, bool* b)
{
	ImguiMenuItemToggle* item = new ImguiMenuItemToggle();
	item->label = label;
	item->function = b;
	fileMenu.push_back(item);
}

void ImguiMenu::AddWindowMenu(std::string label, std::function<void()> function)
{
	ImguiMenuItemFunction* item = new ImguiMenuItemFunction();
	item->label = label;
	item->function = function;
	windowMenu.push_back(item);
}

void ImguiMenu::AddWindowMenu(std::string label, bool* b)
{
	ImguiMenuItemToggle* item = new ImguiMenuItemToggle();
	item->label = label;
	item->function = b;
	windowMenu.push_back(item);
}

void ImguiMenu::AddCustomWindow(ImguiWindow* window)
{
	if (window == NULL)
		return;

	customWindows.push_back(window);
}

void ImguiMenu::DrawMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			for (size_t i = 0; i < fileMenu.size(); i++)
			{
				if (ImGui::MenuItem(fileMenu[i]->label.c_str()))
					fileMenu[i]->Run();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			for (size_t i = 0; i < windowMenu.size(); i++)
			{
				if (ImGui::MenuItem(windowMenu[i]->label.c_str()))
					windowMenu[i]->Run();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Scene Select") )
		{
			if (ImguiMenuSceneManager != NULL) {
				auto name = ImguiMenuSceneManager->GetSceneNames();
				for (size_t i = 0; i < name.size(); i++)
				{
					auto tBool = name[i] == ImguiMenuSceneManager->GetActiveScene()->SceneName;
					auto changed = ImGui::MenuItem(name[i].c_str(), nullptr, &tBool);

					if (changed)
					{
						//cout << "Scene Changed To:" << name[i].c_str() << endl;
						ImguiMenuSceneManager->ChangeScene(name[i]);
					}
				}
			}
			else
			{
				ImGui::MenuItem("sceneManager NULL");
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Custom Windows"))
		{
			for (size_t i = 0; i < customWindows.size(); i++)
				ImGui::MenuItem(customWindows[i]->WindowName.c_str(), nullptr, &customWindows[i]->IsOpen);

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void ImguiMenuItemFunction::Run()
{
	if (function != NULL)
		function();
}

void ImguiMenuItemToggle::Run()
{
	if (function != NULL)
		*function = !*function;
}
