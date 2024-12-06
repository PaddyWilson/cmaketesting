#pragma once

#include <string>

#include "SceneManager.h"

class Game
{
public:
	Game(std::string windowName = "default window name");

	void Init(int width, int height);
	void Run();
	void Stop();

	bool IsRunning();

	void AddScene(BaseScene* scene);

private:
	std::string windowName;

	bool initialized = false;
	bool running = false;

	SceneManager sceneManager;
};

