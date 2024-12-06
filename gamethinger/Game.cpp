#include "Game.h"

#include <raylib.h>

#include "TextureManager.h"
#include "BlankScene.h"

Game::Game(std::string windowName)
{
	this->windowName = windowName;
}

void Game::Init(int width, int height)
{
	// i know you can just or(|) them together
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN);
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	//SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(width, height, windowName.c_str());
	InitAudioDevice();

	initialized = true;
}

void Game::Run()
{
	if (!initialized)
		printf("Game not initialized");

	//if not scenes where add just make a blank one
	if (sceneManager.GetSceneCount() == 0)
		sceneManager.AddScene(new BlankScene());

	auto window = GetWindowHandle();

	running = true;
	while (running)
	{
		//only close by Window close button
		if (WindowShouldClose() && !IsKeyDown(KEY_ESCAPE))
		{
			running = false;
			break;
		}

		//used to load images async
		TextureManager::Update();

		sceneManager.Update(GetFrameTime());

		//end update
		//----------------------------
		//start draw
		BeginDrawing();
		
		ClearBackground(RAYWHITE);
		
		//BeginMode2D(camera);		
		sceneManager.Render();
		//EndMode2D();

		//render ui
		sceneManager.RenderUI();

		//end draw
		EndDrawing();
	}

	sceneManager.End();

	TextureManager::End();
	CloseAudioDevice();
	CloseWindow();
}

void Game::Stop()
{
	running = false;
}

bool Game::IsRunning()
{
	return running;
}

void Game::AddScene(BaseScene* scene)
{
	sceneManager.AddScene(scene);
}
