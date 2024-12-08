#pragma once
#include <imgui.h>
#include <rlImGui.h>
#include <rlImGuiColors.h>
#include <string>
class ImguiWindow
{
public:
	bool IsOpen = false;
	std::string WindowName = "Default Name";	

	virtual void Init() = 0;
	virtual void End() {}
	virtual void Update() {}
	virtual void Draw() {}
};
