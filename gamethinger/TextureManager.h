#pragma once
#include <map>
#include <vector>
//#include <concurrent_queue.h>
#include <queue>
#include <string>
#include <iostream>
#include <raylib.h>
#include <thread>

#include "Global.h"

constexpr auto MAX_TEXTURE_SIZE = 32767;;

 class TextureManager
{
private:
	static std::map<std::string, Texture2D> textures;

	struct LoadData
	{
		std::string alias;
		Image image;
	};

	//a queue to load data from when update is called
	//static concurrency::concurrent_queue<LoadData> loadQueue;
	static std::queue<LoadData> loadQueue;
	static std::thread singleThread;
	static std::thread listThread;

public:
	static void Init();
	static void End();

	static int Load(std::string path, std::string alias);
	static void Load(std::vector<std::string> paths, std::vector<std::string> alias);
	static void Unload(std::string alias);
	static Texture2D GetTexture(std::string alias);

	//for async
	static void Update();
	static void LoadAsync(std::string path, std::string alias);
	static void LoadAsync(std::vector<std::string> paths, std::vector<std::string> alias);
private:
	static void LoadAsyncMain(std::string path, std::string alias);
	static void LoadListAsyncMain(std::vector<std::string> paths, std::vector<std::string> alias);
};

