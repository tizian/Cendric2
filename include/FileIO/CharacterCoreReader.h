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
	CharacterCoreReader();
	// reads the whole character core when onlySaveGame is false and
	// only reads the attributes relevant for a savegame (name, time played and date) if its true
	bool readCharacterCore(const std::string& fileName, CharacterCoreData& data, bool onlySaveGame = false);

private:
	bool readTimePlayed(std::string& line, CharacterCoreData& data) const;
	bool readSavegameName(std::string& line, CharacterCoreData& data) const;
	bool readSavegameDate(std::string& line, CharacterCoreData& data) const;

	bool readIsInLevel(std::string& line, CharacterCoreData& data) const;
	bool readMapID(std::string& line, CharacterCoreData& data) const;
	bool readMapPosition(std::string& line, CharacterCoreData& data) const;
	bool readLevelID(std::string& line, CharacterCoreData& data) const;
	bool readLevelPosition(std::string& line, CharacterCoreData& data) const;
	bool readEnemiesKilled(std::string& line, CharacterCoreData& data) const;
	bool readEnemiesLooted(std::string& line, CharacterCoreData& data) const;
	bool readItemsLooted(std::string& line, CharacterCoreData& data) const;
	bool readChestsLooted(std::string& line, CharacterCoreData& data) const;
	bool readWaypointsUnlocked(std::string& line, CharacterCoreData& data) const;
	bool readTriggersTriggered(std::string& line, CharacterCoreData& data) const;
	bool readTilesExplored(std::string& line, CharacterCoreData& data) const;
	bool readQuestStates(std::string& line, CharacterCoreData& data) const;
	bool readMerchantState(std::string& line, CharacterCoreData& data) const;
	bool readQuestProgressTargets(std::string& line, CharacterCoreData& data) const;
	bool readQuestProgressConditions(std::string& line, CharacterCoreData& data) const;
	bool readQuestProgressDescription(std::string& line, CharacterCoreData& data) const;
	bool readProgressConditions(std::string& line, CharacterCoreData& data) const;
	bool readGuild(std::string& line, CharacterCoreData& data) const;
	bool readReputationProgress(std::string& line, CharacterCoreData& data) const;
	bool readLearnedSpells(std::string& line, CharacterCoreData& data) const;
	bool readLearnedModifiers(std::string& line, CharacterCoreData& data) const;
	bool readLearnedHints(std::string& line, CharacterCoreData& data) const;
	bool readWeather(std::string& line, CharacterCoreData& data) const;

	bool readLevelStateLayer(std::string& line, std::set<int>& layer, std::string& id) const;

	bool readAttributes(std::string& line, CharacterCoreData& data) const;

	bool readGold(std::string& line, CharacterCoreData& data) const;
	bool readItemID(std::string& line, CharacterCoreData& data) const;
	bool readStoredGold(std::string& line, CharacterCoreData& data) const;
	bool readStoredItemID(std::string& line, CharacterCoreData& data) const;
	bool readEquippedWeaponSlots(std::string& line, CharacterCoreData& data) const;
	bool readWeaponConfigurations(std::string& line, CharacterCoreData& data) const;
	bool readWeaponSpell(std::string& line, CharacterCoreData& data) const;
	bool readQuickslot(std::string& line, CharacterCoreData& data) const;

	bool readEquippedWeapon(std::string& line, CharacterCoreData& data) const;
	bool readEquippedHead(std::string& line, CharacterCoreData& data) const;
	bool readEquippedNeck(std::string& line, CharacterCoreData& data) const;
	bool readEquippedBody(std::string& line, CharacterCoreData& data) const;
	bool readEquippedBack(std::string& line, CharacterCoreData& data) const;
	bool readEquippedRing1(std::string& line, CharacterCoreData& data) const;
	bool readEquippedRing2(std::string& line, CharacterCoreData& data) const;

	// check map bean for validity before loading the savegame
	bool checkData(CharacterCoreData& data) const;

private:
	typedef bool (CharacterCoreReader::*ReadFunction)(std::string&, CharacterCoreData&) const;

	std::map<std::string, ReadFunction> m_readMap;
	void initReadMap();

	bool m_stopReading = false;
};