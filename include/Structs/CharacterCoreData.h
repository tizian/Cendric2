#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Enums/MapID.h"
#include "Enums/LevelID.h"
#include "Enums/NPCID.h"
#include "Enums/NPCState.h"
#include "Enums/QuestID.h"
#include "Enums/QuestState.h"

#include "Structs/AttributeBean.h"

struct CharacterCoreData
{
	// savegame attributes
	sf::Time timePlayed;
	bool isQuicksave;
	std::string saveGameName;
	std::time_t dateSaved;

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
	std::string equippedWeapon;
	std::string equippedHead;
	std::string equippedBody;
	std::string equippedBack;
	std::string equippedRing1;
	std::string equippedRing2;
	std::string equippedNeck;
	std::map<std::string, int> items;
};

const CharacterCoreData DEFAULT_CORE
{
	sf::Time::Zero,
	false,
	"",
	time(nullptr),
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
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	std::map<std::string, int>()
};