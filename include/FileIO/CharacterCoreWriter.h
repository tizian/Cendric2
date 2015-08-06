#pragma once

#include <fstream>

#include "global.h"
#include "Logger.h"

#include "Enums/ItemType.h"
#include "Structs/CharacterCoreData.h"
#include "FileIO/CharacterCoreIO.h"

// a writer to write a savefile.
class CharacterCoreWriter : public CharacterCoreIO
{
public:
	bool saveToFile(const std::string& filename, const CharacterCoreData& data) const;
	
	// tries to create file with that filename and returns true if successful.
	// if file already exists, it returns false.
	bool createFile(const std::string& filename) const;
	
private:
	std::string writeTimePlayed(const CharacterCoreData& data) const;
	std::string writeSaveGameName(const CharacterCoreData& data) const;
	std::string writeDateSaved(const CharacterCoreData& data) const;

	std::string writeMapID(const CharacterCoreData& data) const;
	std::string writeMapPosition(const CharacterCoreData& data) const;
	std::string writeLevelID(const CharacterCoreData& data) const;
	std::string writeLevelPosition(const CharacterCoreData& data) const;
	std::string writeLevelKilled(const CharacterCoreData& data) const;
	std::string writeLevelLooted(const CharacterCoreData& data) const;
	std::string writeQuestStates(const CharacterCoreData& data) const;
	std::string writeNPCStates(const CharacterCoreData& data) const;

	std::string writeAttributes(const CharacterCoreData& data) const;

	std::string writeGold(const CharacterCoreData& data) const;
	std::string writeItemID(const CharacterCoreData& data) const;
	std::string writeEquippedWeaponSlots(const CharacterCoreData& data) const;
	std::string writeEquippedItems(const CharacterCoreData& data) const;
};