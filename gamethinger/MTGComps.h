#pragma once

#include <string>
#include <vector>

class MTGCard;

class MTGDeck
{
public:
	std::string name;
	std::string description;

	std::vector<MTGCard> cards;

	std::string json;
private:
};

class MTGPlayer {
private:
public:
	std::string displayName;
};

class MTGCard
{
private:

public:
	enum class Location {
		NONE, DECK, HAND, BATTLEFIELD, GRAVE, EXILE, COMMANDER, TOKEN, REMOVE
	};
	enum class Type {
		NONE, LAND, CREATURE, ARTIFACT, ENCHANTMENT, PLANESWALKER, INSTANT, SCORCEY, TOKEN
	};

	std::string name = "";
	std::string type = "";
	std::string id = "";

	Type cardType = Type::NONE;
	Location location = Location::NONE;

	int counters = 0;

	MTGPlayer* owner = nullptr;

	bool fliped = false;
	std::string front = "";
	std::string back = "";
};