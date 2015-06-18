#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Enums/MapID.h"
#include "Enums/LevelID.h"
#include "Enums/ItemID.h"

#include "Structs/AttributeBean.h"

struct CharacterCoreData
{
	// player attributes
	std::string playerName;
	sf::Time timePlayed;

	// position & progress
	MapID currentMap;
	sf::Vector2f currentMapPosition;
	LevelID currentLevel;
	sf::Vector2f currentLevelPosition;
	std::map<LevelID, std::vector<bool>> levelLooted;
	std::map<LevelID, std::vector<bool>> levelKilled;
	// TODO: quest progress
	
	// base attributes
	AttributeBean attributes;

	// items & equipment
	int gold;
	ItemID equippedWeapon;
	ItemID equippedHead;
	ItemID equippedBody;
	ItemID equippedBack;
	ItemID equippedRing1;
	ItemID equippedRing2;
	ItemID equippedNeck;
	std::map<ItemID, int> items;
};

const CharacterCoreData DEFAULT_CORE
{
	"",
	sf::Time::Zero,
	MapID::Void,
	sf::Vector2f(),
	LevelID::Void,
	sf::Vector2f(),
	std::map<LevelID, std::vector<bool>>(),
	std::map<LevelID, std::vector<bool>>(),
	ZERO_ATTRIBUTES,
	0,
	ItemID::Void,
	ItemID::Void,
	ItemID::Void,
	ItemID::Void,
	ItemID::Void,
	ItemID::Void,
	ItemID::Void,
	std::map<ItemID, int>()
};