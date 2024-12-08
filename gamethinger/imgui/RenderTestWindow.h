#pragma once
#include <raylib.h>

#include "ImguiWindow.h"
class RenderTestWindow : public ImguiWindow
{
private:
	RenderTexture2D texture = LoadRenderTexture(100, 100);

public:
	void Init();
	void Update();
	void Draw();
	void End();
};
