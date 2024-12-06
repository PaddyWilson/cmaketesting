#pragma once

#include <string>
namespace Global
{
	static std::string BasePath = "C:\\Users\\GGGGG\\Desktop\\Code\\GameThinger\\Content\\";
	static std::string DeckPath = BasePath + "Decks\\";
	static std::string TexturePath = BasePath + "Textures\\";
	static std::string AudioPath = BasePath + "Audio\\";
	static std::string ConfigFile = BasePath + "config.txt";

	const std::string DefaultTextureName = "NO-TEXTURE.png";

	static int ScreenWidth = 1920;
	static int ScreenHeight = 1080;
}
