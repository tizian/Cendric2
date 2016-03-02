#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <set>

#include "Enums/QuestState.h"
#include "Enums/SpellID.h"
#include "Enums/SpellType.h"

#include "Structs/SpellModifier.h"
#include "Structs/AttributeData.h"
#include "Structs/MerchantData.h"

struct CharacterCoreData {
	// savegame attributes
	sf::Time timePlayed;
	std::string saveGameName;
	std::time_t dateSaved;

	// position & progress
	bool isInLevel;
	std::string currentMap;
	sf::Vector2f currentMapPosition;
	std::string currentLevel;
	sf::Vector2f currentLevelPosition;
	std::map<std::string, std::set<int>> enemiesLooted;
	std::map<std::string, std::set<int>> enemiesKilled;
	std::map<std::string, std::set<int>> itemsLooted;
	std::map<std::string, std::set<int>> chestsLooted;
	std::map<std::string, std::set<int>> waypointsUnlocked;

	// quest & npc progress
	std::map<std::string, std::set<std::string>> conditionProgress;
	std::map<std::string, MerchantData> merchantStates;
	std::map<std::string, QuestState> questStates;
	std::map<std::string, std::map<std::string, int>> questTargetProgress;
	std::map<std::string, std::set<std::string>> questConditionProgress;

	// learning progress
	std::map<SpellType, std::set<SpellID>> spellsLearned;
	std::map<SpellModifierType, int> modfiersLearned;

	// base attributes
	AttributeData attributes;

	// items & equipment
	int gold;
	std::vector<std::pair<SpellID, std::vector<SpellModifier>>> equippedWeaponSlots;
	std::string equippedWeapon;
	std::string equippedHead;
	std::string equippedBody;
	std::string equippedBack;
	std::string equippedRing1;
	std::string equippedRing2;
	std::string equippedNeck;
	std::map<std::string, int> items;

	// quickslot assignment
	std::string quickSlot1;
	std::string quickSlot2;
};

const CharacterCoreData DEFAULT_CORE
{
	sf::Time::Zero,
	"",
	time(nullptr),
	false,
	"",
	sf::Vector2f(),
	"",
	sf::Vector2f(),
	std::map<std::string, std::set<int>>(),
	std::map<std::string, std::set<int>>(),
	std::map<std::string, std::set<int>>(),
	std::map<std::string, std::set<int>>(),
	std::map<std::string, std::set<int>>(),
	std::map<std::string, std::set<std::string>>(),
	std::map<std::string, MerchantData>(),
	std::map<std::string, QuestState>(),
	std::map<std::string, std::map<std::string, int>>(),
	std::map<std::string, std::set<std::string>>(),
	std::map<SpellType, std::set<SpellID>>(),
	std::map<SpellModifierType, int>(),
	ZERO_ATTRIBUTES,
	0,
	std::vector<std::pair<SpellID, std::vector<SpellModifier>>>(),
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	std::map<std::string, int>(),

	"",
	"",
};