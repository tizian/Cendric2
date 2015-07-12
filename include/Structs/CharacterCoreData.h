#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Enums/MapID.h"
#include "Enums/LevelID.h"
#include "Enums/ItemID.h"
#include "Enums/NPCID.h"
#include "Enums/NPCState.h"
#include "Enums/QuestID.h"
#include "Enums/QuestState.h"

#include "Structs/AttributeBean.h"

struct CharacterCoreData
{
	// player attributes
	sf::Time timePlayed;

	// position & progress
	MapID currentMap;
	sf::Vector2f currentMapPosition;
	LevelID currentLevel;
	sf::Vector2f currentLevelPosition;
	std::map<LevelID, std::vector<bool>> levelLooted;
	std::map<LevelID, std::vector<bool>> levelKilled;
	
	// quest & npc progress
	std::map<NPCID, NPCState> npcStates;
	std::map<QuestID, QuestState> questStates;
	
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
	sf::Time::Zero,
	MapID::VOID,
	sf::Vector2f(),
	LevelID::VOID,
	sf::Vector2f(),
	std::map<LevelID, std::vector<bool>>(),
	std::map<LevelID, std::vector<bool>>(),
	std::map<NPCID, NPCState>(),
	std::map<QuestID, QuestState>(),
	ZERO_ATTRIBUTES,
	0,
	ItemID::VOID,
	ItemID::VOID,
	ItemID::VOID,
	ItemID::VOID,
	ItemID::VOID,
	ItemID::VOID,
	ItemID::VOID,
	std::map<ItemID, int>()
};