#pragma once

#include "global.h"

#include "Enums/QuestState.h"
#include "Enums/SpellType.h"
#include "Enums/FractionID.h"
#include "Enums/Key.h"

#include "Structs/SpellModifier.h"
#include "Structs/AttributeData.h"
#include "Structs/MerchantData.h"
#include "Structs/WeatherData.h"

typedef std::map<std::string, std::pair<sf::Vector2i, std::vector<bool>>> ExploredTiles;

struct CharacterCoreData {
	// savegame attributes
	sf::Time timePlayed;
	std::string saveGameName;
	std::time_t dateSaved = time(nullptr);

	// position & progress
	FractionID guild = FractionID::VOID;
	bool isInLevel = false;
	std::string currentMap;
	sf::Vector2f currentMapPosition;
	std::string currentLevel;
	sf::Vector2f currentLevelPosition;
	std::string forcedMap;
	sf::Vector2f forcedMapPosition;
	std::map<std::string, std::set<int>> enemiesLooted;
	std::map<std::string, std::set<int>> enemiesKilled;
	std::map<std::string, std::set<int>> itemsLooted;
	std::map<std::string, std::set<int>> chestsLooted;
	std::map<std::string, std::map<int, sf::Vector2f>> waypointsUnlocked;
	std::map<std::string, std::set<int>> triggersTriggered;
	ExploredTiles tilesExplored;

	// quest & npc progress
	std::map<std::string, std::set<std::string>> conditionProgress;
	std::map<std::string, std::map<std::string, int>> merchantStates;
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
	AttributeData attributes = ZERO_ATTRIBUTES;

	// items & equipment
	int gold = 0;
	int storedGold = 0;
	std::vector<WeaponSlot> equippedWeaponSlots;
	std::map<ItemType, std::string> equippedItems;
	std::map<std::string, int> items;
	std::map<std::string, int> storedItems;

	// weapon configurations
	std::map<std::string, std::vector<WeaponSlot>> weaponConfigurations;
	Key weaponSpell = Key::Chop;

	// quickslot assignment
	std::string quickSlot1;
	std::string quickSlot2;
};
