#include "RenderTestWindow.h"

void RenderTestWindow::Init()
{
	WindowName = "RenderTestWindow";
}

void RenderTestWindow::Update()
{
	if (!IsOpen)
		return;

	BeginTextureMode(texture);
	ClearBackground(SKYBLUE);
	//render to texture
	//can use a other camera if needed
	//BeginMode2D(some camera);
	//EndMode2D();

	DrawRectangle(10, 10, 90, 90, PINK);
	EndTextureMode();
}

void RenderTestWindow::Draw()
{
	if (!IsOpen)
		return;

	ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2(400, 400));
	if (ImGui::Begin(WindowName.c_str(), &IsOpen, ImGuiWindowFlags_NoResize))
	{
		ImGuiWindowFlags_NoResize;
		//Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);

		ImVec2 size = ImGui::GetContentRegionAvail();

		Rectangle viewRect = { 0 };
		viewRect.x = texture.texture.width / 2 - size.x / 2;
		viewRect.y = texture.texture.height / 2 - size.y / 2;
		viewRect.width = size.x;
		viewRect.height = -size.y;

		// draw the view
		rlImGuiImageRect(&texture.texture, (int)size.x, (int)size.y, viewRect);

		ImGui::End();
	}
}

void RenderTestWindow::End()
{
}
