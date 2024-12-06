#include "TextureManager.h"

std::map<std::string, Texture2D> TextureManager::textures;

concurrency::concurrent_queue<TextureManager::LoadData> TextureManager::loadQueue;
std::thread TextureManager::singleThread;
std::thread TextureManager::listThread;

void TextureManager::Init() {
	Load(Global::BasePath + Global::DefaultTextureName, Global::DefaultTextureName);
}

void TextureManager::End()
{
	//unload all textures
	for (auto const& tex : textures)
		UnloadTexture(tex.second);

	if (singleThread.joinable())
		singleThread.join();
	if (listThread.joinable())
		listThread.join();
}

int TextureManager::Load(std::string path, std::string alias)
{
	if (!FileExists(path.c_str()))
		return 1;
	textures[alias] = LoadTexture(path.c_str());
	SetTextureFilter(textures[alias], TEXTURE_FILTER_ANISOTROPIC_16X);

	//GenTextureMipmaps(&textures[alias]);

	return 0;
}

void TextureManager::Load(std::vector<std::string> paths, std::vector<std::string> alias)
{
	if (paths.size() != alias.size())
		return;

	for (size_t i = 0; i < paths.size(); i++)
		Load(paths[i], alias[i]);
}

void TextureManager::Unload(std::string alias)
{
	//can't find texture
	if (textures.find(alias) == textures.end())
		return;

	UnloadTexture(textures[Global::DefaultTextureName]);
}

Texture2D TextureManager::GetTexture(std::string alias)
{
	//can't find texture
	if (textures.find(alias) == textures.end())
		return textures[Global::DefaultTextureName];
	//found texture
	return textures[alias];
}


void TextureManager::Update()
{
	if (loadQueue.empty())
		return;

	LoadData data;
	if (!loadQueue.try_pop(data))
		return;

	textures[data.alias] = LoadTextureFromImage(data.image);
	SetTextureFilter(textures[data.alias], TEXTURE_FILTER_ANISOTROPIC_16X);
	UnloadImage(data.image);
}

void TextureManager::LoadAsync(std::string path, std::string alias)
{
	singleThread = std::thread(LoadAsyncMain, path, alias);
	//you can detach now or wait for join before distorying thread
	singleThread.detach();
}

void TextureManager::LoadAsync(std::vector<std::string> paths, std::vector<std::string> alias)
{
	listThread =  std::thread(LoadListAsyncMain, paths, alias);
	listThread.detach();
}


void TextureManager::LoadAsyncMain(std::string path, std::string alias)
{
	LoadData data;
	data.alias = alias;
	data.image = LoadImage(path.c_str());

	//ImageResize(&data.image, data.image.width / 2, data.image.height / 2);

	loadQueue.push(data);
}

void TextureManager::LoadListAsyncMain(std::vector<std::string> paths, std::vector<std::string> alias)
{
	for (size_t i = 0; i < paths.size(); i++)
	{
		LoadData data;
		data.alias = alias[i];
		data.image = LoadImage(paths[i].c_str());
		loadQueue.push(data);
	}	
}

