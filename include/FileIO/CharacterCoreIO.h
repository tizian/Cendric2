#pragma once

#include "global.h"

// some constants used by character core reader & writer
class CharacterCoreIO {
public:
	virtual ~CharacterCoreIO() {};
protected:
	std::string hashFile(const std::string& input) const;
protected:
	// savegame attributes
	const char* TIME_PLAYED = "time.played";
	const char* SAVE_GAME_NAME = "savegame.name";
	const char* DATE_SAVED = "savegame.date";

	// attributes
	const char* ATTRIBUTES = "attributes";

	// position & progress
	const char* IS_IN_LEVEL = "is.in.level";
	const char* OVERWORLD_ID = "overworld.id";
	const char* MAP_ID = "map.id";
	const char* MAP_POSITION = "map.position";
	const char* FORCED_MAP_ID = "forced.map.id";
	const char* FORCED_MAP_POSITION = "forced.map.position";
	const char* LEVEL_ID = "level.id";
	const char* LEVEL_POSITION = "level.position";
	const char* ENEMIES_KILLED = "enemies.killed";
	const char* ENEMIES_LOOTED = "enemies.looted";
	const char* ITEMS_LOOTED = "items.looted";
	const char* CHESTS_LOOTED = "chests.looted";
	const char* BOOKS_READ = "books.read";
	const char* MODIFIERS_UNLOCKED = "modifiers.unlocked";
	const char* WAYPOINTS_UNLOCKED = "waypoints.unlocked";
	const char* TRIGGERS_TRIGGERED = "triggers.triggered";
	const char* DOORS_OPEN = "doors.open";
	const char* TILES_EXPLORED = "tiles.explored";
	const char* MERCHANT_STATE = "merchant.state";
	const char* QUEST_STATE = "quest.state";
	const char* QUEST_PROGRESS_TARGET = "quest.progress.target";
	const char* QUEST_PROGRESS_CONDITION = "quest.progress.condition";
	const char* QUEST_PROGRESS_DESCRIPTION = "quest.progress.description";
	const char* PROGRESS_CONDITION = "progress.condition";
	const char* SPELL_LEARNED = "spell.learned";
	const char* MODIFIER_LEARNED = "modifier.learned";
	const char* HINT_LEARNED = "hint.learned";
	const char* ACHIEVEMENT_UNLOCKED = "achievement.unlocked";
	const char* GUILD = "guild";
	const char* REPUTATION_PROGRESS = "reputation.progress";
	const char* WEATHER = "weather";
	const char* DEATH_COUNT = "deaths";
	const char* DEATH_LEVEL = "level.deaths";

	// equipment & items
	const char* GOLD = "gold";
	const char* STORED_GOLD = "stored.gold";
	const char* ITEM_ID = "item.id";
	const char* STORED_ITEM_ID = "stored.item.id";
	const char* EQUIPPED_SPELLSLOT = "equipped.spellslot";
	const char* EQUIPPED_WEAPON = "equipped.weapon";
	const char* EQUIPPED_HEAD = "equipped.head";
	const char* EQUIPPED_BODY = "equipped.body";
	const char* EQUIPPED_RING_1 = "equipped.ring1";
	const char* EQUIPPED_RING_2 = "equipped.ring2";
	const char* EQUIPPED_NECK = "equipped.neck";
	const char* EQUIPPED_BACK = "equipped.back";

	// weapon configurations
	const char* WEAPON_CONFIGS = "weapon.configs";
	const char* WEAPON_SPELL = "weapon.spell";

	// quickslot assignment
	const char* QUICKSLOT = "quickslot";

	// hash
	const char* HASH = "hash";
};