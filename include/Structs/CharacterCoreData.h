#pragma once

#include "global.h"

#include "Enums/QuestState.h"
#include "Enums/SpellID.h"
#include "Enums/SpellType.h"
#include "Enums/FractionID.h"

#include "Structs/SpellModifier.h"
#include "Structs/AttributeData.h"
#include "Structs/MerchantData.h"
#include "Structs/WeatherData.h"

struct CharacterCoreData {
	// savegame attributes
	sf::Time timePlayed;
	std::string saveGameName;
	std::time_t dateSaved;

	// position & progress
	FractionID guild;
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
	std::map<std::string, std::set<int>> triggersTriggered;
	std::map<std::string, std::vector<bool>> tilesExplored;

	// quest & npc progress
	std::map<std::string, std::set<std::string>> conditionProgress;
	std::map<std::string, MerchantData> merchantStates;
	std::map<std::string, QuestState> questStates;
	std::map<std::string, std::map<std::string, int>> questTargetProgress;
	std::map<std::string, std::set<std::string>> questConditionProgress;
	std::map<std::string, std::set<int>> questDescriptionProgress;

	// reputation progress
	std::map<FractionID, int> reputationProgress;

	// learning progress
	std::map<SpellType, std::set<SpellID>> spellsLearned;
	std::map<SpellModifierType, int> modfiersLearned;

	// hints
	std::vector<std::string> hintsLearned;

	// weather
	std::map<std::string, WeatherData> currentWeather;

	// base attributes
	AttributeData attributes;

	// items & equipment
	int gold;
	int storedGold;
	std::vector<std::pair<SpellID, std::vector<SpellModifier>>> equippedWeaponSlots;
	std::map<ItemType, std::string> equippedItems;
	std::map<std::string, int> items;
	std::map<std::string, int> storedItems;

	// weapon configurations
	std::map<std::string, std::vector<std::pair<SpellID, std::vector<SpellModifier>>>> weaponConfigurations;

	// quickslot assignment
	std::string quickSlot1;
	std::string quickSlot2;
};

const CharacterCoreData DEFAULT_CORE
{
	sf::Time::Zero,
	"",
	time(nullptr),
	FractionID::VOID,
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
	std::map<std::string, std::set<int>>(),
	std::map<std::string, std::vector<bool>>(),
	std::map<std::string, std::set<std::string>>(),
	std::map<std::string, MerchantData>(),
	std::map<std::string, QuestState>(),
	std::map<std::string, std::map<std::string, int>>(),
	std::map<std::string, std::set<std::string>>(),
	std::map<std::string, std::set<int>>(),
	std::map<FractionID, int>(),
	std::map<SpellType, std::set<SpellID>>(),
	std::map<SpellModifierType, int>(),
	std::vector<std::string>(),
	std::map<std::string, WeatherData>(),
	ZERO_ATTRIBUTES,
	0,
	0,
	std::vector<std::pair<SpellID, std::vector<SpellModifier>>>(),
	std::map<ItemType, std::string>(),
	std::map<std::string, int>(),
	std::map<std::string, int>(),
	std::map<std::string, std::vector<std::pair<SpellID, std::vector<SpellModifier>>>>(),
	"",
	"",
};