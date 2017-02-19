#pragma once

#include "global.h"
#include "FileIO/Reader.h"
#include "Logger.h"

#include "Enums/ItemType.h"
#include "Structs/CharacterCoreData.h"
#include "FileIO/CharacterCoreIO.h"

// a reader to read a savegame.
class CharacterCoreReader : public Reader, public CharacterCoreIO {
public:
	// reads the whole character core when onlySaveGame is false and
	// only reads the attributes relevant for a savegame (name, time played and date) if its true
	bool readCharacterCore(const std::string& fileName, CharacterCoreData& data, bool onlySaveGame = false);

private:
	bool readTimePlayed(char* start, char* end, CharacterCoreData& data) const;
	bool readSavegameName(char* start, char* end, CharacterCoreData& data) const;
	bool readSavegameDate(char* start, char* end, CharacterCoreData& data) const;

	bool readIsInLevel(char* start, char* end, CharacterCoreData& data) const;
	bool readMapID(char* start, char* end, CharacterCoreData& data) const;
	bool readMapPosition(char* start, char* end, CharacterCoreData& data) const;
	bool readLevelID(char* start, char* end, CharacterCoreData& data) const;
	bool readLevelPosition(char* start, char* end, CharacterCoreData& data) const;
	bool readEnemiesKilled(char* start, char* end, CharacterCoreData& data) const;
	bool readEnemiesLooted(char* start, char* end, CharacterCoreData& data) const;
	bool readItemsLooted(char* start, char* end, CharacterCoreData& data) const;
	bool readChestsLooted(char* start, char* end, CharacterCoreData& data) const;
	bool readWaypointsUnlocked(char* start, char* end, CharacterCoreData& data) const;
	bool readTriggersTriggered(char* start, char* end, CharacterCoreData& data) const;
	bool readTilesExplored(char* start, char* end, CharacterCoreData& data) const;
	bool readQuestStates(char* start, char* end, CharacterCoreData& data) const;
	bool readMerchantState(char* start, char* end, CharacterCoreData& data) const;
	bool readQuestProgressTargets(char* start, char* end, CharacterCoreData& data) const;
	bool readQuestProgressConditions(char* start, char* end, CharacterCoreData& data) const;
	bool readQuestProgressDescription(char* start, char* end, CharacterCoreData& data) const;
	bool readProgressConditions(char* start, char* end, CharacterCoreData& data) const;
	bool readGuild(char* start, char* end, CharacterCoreData& data) const;
	bool readReputationProgress(char* start, char* end, CharacterCoreData& data) const;
	bool readLearnedSpells(char* start, char* end, CharacterCoreData& data) const;
	bool readLearnedModifiers(char* start, char* end, CharacterCoreData& data) const;
	bool readLearnedHints(char* start, char* end, CharacterCoreData& data) const;
	bool readWeather(char* start, char* end, CharacterCoreData& data) const;

	bool readLevelStateLayer(char* start, char* end, std::set<int>& layer, std::string& id) const;

	bool readAttributes(char* start, char* end, CharacterCoreData& data) const;

	bool readGold(char* start, char* end, CharacterCoreData& data) const;
	bool readItemID(char* start, char* end, CharacterCoreData& data) const;
	bool readStoredGold(char* start, char* end, CharacterCoreData& data) const;
	bool readStoredItemID(char* start, char* end, CharacterCoreData& data) const;
	bool readEquippedWeaponSlots(char* start, char* end, CharacterCoreData& data) const;
	bool readEquippedItem(char* start, char* end, CharacterCoreData& data, ItemType type) const;
	bool readWeaponConfigurations(char* start, char* end, CharacterCoreData& data) const;
	bool readWeaponSpell(char* start, char* end, CharacterCoreData& data) const;
	bool readQuickslot(char* start, char* end, CharacterCoreData& data) const;

	// check map bean for validity before loading the savegame
	bool checkData(CharacterCoreData& data) const;
};