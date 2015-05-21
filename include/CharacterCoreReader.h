#pragma once

#include "global.h"
#include "Reader.h"
#include "Logger.h"

#include "Enums/ItemType.h"
#include "Structs/CharacterCoreData.h"

// a reader to read a savegame.
class CharacterCoreReader : public Reader
{
public:
	CharacterCoreReader();
	~CharacterCoreReader();

	bool readCharacterCore(char* fileName, CharacterCoreData& data);

private:

	bool readPlayerName(char* start, char* end, CharacterCoreData& data) const;
	bool readTimePlayed(char* start, char* end, CharacterCoreData& data) const;

	bool readMapID(char* start, char* end, CharacterCoreData& data) const;
	bool readMapPosition(char* start, char* end, CharacterCoreData& data) const;
	bool readLevelKilled(char* start, char* end, CharacterCoreData& data) const;
	bool readLevelLooted(char* start, char* end, CharacterCoreData& data) const;
	
	bool readGold(char* start, char* end, CharacterCoreData& data) const;
	bool readItemID(char* start, char* end, CharacterCoreData& data) const;
	bool readEquippedItem(char* start, char* end, CharacterCoreData& data, ItemType type) const;

	// \brief check map bean for validity before loading the savegame
	bool checkData(CharacterCoreData& data) const;

	// update data to prepare it for game
	void updateData(CharacterCoreData& data) const;

	const char* PLAYER_NAME = "player.name";
	const char* TIME_PLAYED = "time.played";

	// position & progress
	const char* MAP_ID = "map.id";
	const char* MAP_POSITION = "map.position";
	const char* LEVEL_KILLED = "level.killed";
	const char* LEVEL_LOOTED = "level.looted";

	// equipment
	const char* GOLD = "gold";
	const char* ITEM_ID = "item.id";
	const char* EQUIPPED_WEAPON = "equipped.weapon";
	const char* EQUIPPED_HEAD = "equipped.head";
	const char* EQUIPPED_BODY = "equipped.body";
	const char* EQUIPPED_RING_1 = "equipped.ring1";
	const char* EQUIPPED_RING_2 = "equipped.ring2";
	const char* EQUIPPED_NECK = "equipped.neck";
	const char* EQUIPPED_BACK = "equipped.back";
};