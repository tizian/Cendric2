#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Enums/MapID.h"
#include "Enums/LevelID.h"
#include "Enums/ItemID.h"

#include "Structs/AttributeBean.h"

struct CharacterCoreData
{
	std::string playerName;
	sf::Time timePlayed;

	MapID currentMap;
	sf::Vector2f currentMapPosition;
	std::map<LevelID, std::vector<bool>> levelLooted;
	std::map<LevelID, std::vector<bool>> levelKilled;

	AttributeBean attributes;

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