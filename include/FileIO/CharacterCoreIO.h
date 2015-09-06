#pragma once

#include "global.h"

// some constants used by character core reader & writer
class CharacterCoreIO
{
protected:
	// savegame attributes
	const char* TIME_PLAYED = "time.played";
	const char* SAVE_GAME_NAME = "savegame.name";
	const char* DATE_SAVED = "savegame.date";

	// attributes
	const char* ATTRIBUTES = "attributes";

	// position & progress
	const char* IS_IN_LEVEL = "is.in.level";
	const char* MAP_ID = "map.id";
	const char* MAP_POSITION = "map.position";
	const char* LEVEL_ID = "level.id";
	const char* LEVEL_POSITION = "level.position";
	const char* ENEMIES_KILLED = "enemies.killed";
	const char* ENEMIES_LOOTED = "enemies.looted";
	const char* ITEMS_LOOTED = "items.looted";
	const char* CHESTS_LOOTED = "chests.looted";
	const char* QUEST_STATE = "quest.state";
	const char* NPC_STATE = "npc.state";

	// equipment & items
	const char* GOLD = "gold";
	const char* ITEM_ID = "item.id";
	const char* EQUIPPED_SPELLSLOT = "equipped.spellslot";
	const char* EQUIPPED_WEAPON = "equipped.weapon";
	const char* EQUIPPED_HEAD = "equipped.head";
	const char* EQUIPPED_BODY = "equipped.body";
	const char* EQUIPPED_RING_1 = "equipped.ring1";
	const char* EQUIPPED_RING_2 = "equipped.ring2";
	const char* EQUIPPED_NECK = "equipped.neck";
	const char* EQUIPPED_BACK = "equipped.back";

	// quickslot assignment
	const char* QUICKSLOT = "quickslot";
};