#pragma once

#include "ImguiWindow.h"
#include "../SystemManager.h"

class ImguiSystemsWindow :  public ImguiWindow
{
public:
	SystemManager* systemManager;

	ImguiSystemsWindow(){
		WindowName = "Imgui System Window";
	}

	void Init() {};

	void Draw();
};

