#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <raylib.h>

#include "rapidjson/document.h"
//#include "rapidjson/writer.h"
//#include "rapidjson/stringbuffer.h"

#include "Global.h"
#include "MTGComps.h"

#include "Downloader.h"
#include "Utils.h"
#include "TextureManager.h"

using namespace rapidjson;

class ScryfallJson
{
public:
	static MTGDeck CreateDeck(std::string json)
	{
		MTGDeck deck;

		Document d;
		d.Parse(json.c_str());

		Value& val = d["name"];
		deck.name = val.GetString();
		val = d["description"];
		deck.description = val.GetString();

		ParseCards(d["entries"]["commanders"], &deck);
		ParseCards(d["entries"]["lands"], &deck);
		ParseCards(d["entries"]["nonlands"], &deck);
		ParseCards(d["entries"]["outside"], &deck);

		std::cout << deck.name << " - " << deck.description << " - " << deck.cards.size() << std::endl;
		deck.json = json;

		//DownloadImages(json);
		//LoadTextures(json);

		return deck;
	}

	static void DownloadImages(std::string json)
	{
		Document d;
		d.Parse(json.c_str());

		std::vector<std::string> urls;
		std::vector<std::string> savePaths;

		GetLinks(d["entries"]["commanders"], &urls, &savePaths);
		GetLinks(d["entries"]["lands"], &urls, &savePaths);
		GetLinks(d["entries"]["nonlands"], &urls, &savePaths);
		GetLinks(d["entries"]["outside"], &urls, &savePaths);

		std::cout << "Downloading " << urls.size() << " Items." << std::endl;
		Downloader::DownloadList(urls, savePaths);
	}

	static void LoadTextures(std::string json)
	{
		Document d;
		d.Parse(json.c_str());

		std::vector<std::string> alias;
		std::vector<std::string> savePaths;

		GetPaths(d["entries"]["commanders"], &savePaths, &alias);
		GetPaths(d["entries"]["lands"], &savePaths, &alias);
		GetPaths(d["entries"]["nonlands"], &savePaths, &alias);
		GetPaths(d["entries"]["outside"], &savePaths, &alias);

		std::cout << "Loading " << alias.size() << " Items." << std::endl;

		for (size_t i = 0; i < savePaths.size(); i++)
			alias[i] = GetFileName(savePaths[i].c_str());

		TextureManager::LoadAsync(savePaths, alias);
	}

private:
	static void ParseCards(Value& ar, MTGDeck* deck)
	{
		assert(ar.IsArray());
		for (SizeType i = 0; i < ar.Size(); i++)
		{
			if (ar[i]["card_digest"].IsNull())
				continue;

			for (size_t j = 0; j < ar[i]["count"].GetInt(); j++)
			{
				MTGCard card;

				Value& cd = ar[i]["card_digest"];
				card.name = cd["name"].GetString();
				card.id = cd["id"].GetString();
				card.type = cd["type_line"].GetString();
				card.front = card.id + "_front.png";// cd["image_uris"]["front"].GetString();
				card.back = card.id + "_back.png"; // cd["image_uris"]["front"].GetString();

				deck->cards.push_back(card);
			}
		}
	}

	static void GetLinks(Value& ar, std::vector<std::string>* urlList, std::vector<std::string>* saveFileList)
	{
		for (SizeType i = 0; i < ar.Size(); i++)
		{
			if (ar[i]["card_digest"].IsNull())
				continue;

			Value& cd = ar[i]["card_digest"];
			std::string id = cd["id"].GetString();

			//front image
			std::string dl = cd["image_uris"]["front"].GetString();
			StringReplace(&dl, "/large/", "/png/");
			StringReplace(&dl, ".jpg?", ".png?");

			//only download if not downloaded
			if (!FileExists((Global::TexturePath + id + "_front.png").c_str())) {
				urlList->push_back(dl);
				saveFileList->push_back(Global::TexturePath + id + "_front.png");
			}

			//backimage
			if (cd["image_uris"].HasMember("back")) {
				dl = cd["image_uris"]["back"].GetString();
				StringReplace(&dl, "/large/", "/png/");
				StringReplace(&dl, ".jpg?", ".png?");

				//only download if not downloaded
				if (!FileExists((Global::TexturePath + id + "_back.png").c_str())) {
					urlList->push_back(dl);
					saveFileList->push_back(Global::TexturePath + id + "_back.png");
				}
			}
		}
	}

	static void GetPaths(Value& ar, std::vector<std::string>* pathList, std::vector<std::string>* aliasList)
	{
		for (SizeType i = 0; i < ar.Size(); i++)
		{
			if (ar[i]["card_digest"].IsNull())
				continue;

			Value& cd = ar[i]["card_digest"];
			std::string id = cd["id"].GetString();

			//front image
			pathList->push_back(Global::TexturePath + id + "_front.png");
			aliasList->push_back(id + "_front.png");

			//backimage
			if (cd["image_uris"].HasMember("back"))
			{
				pathList->push_back(Global::TexturePath + id + "_back.png");
				aliasList->push_back(id + "_back.png");
			}
		}
	}
};

