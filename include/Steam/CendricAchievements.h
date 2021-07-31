#pragma once

#include "Steam/AchievementData.h"

// Defining our achievements
enum AchievementID
{
	VOID = 0,
	// bosses
	ACH_SAVE_ZEFF,
	ACH_KILL_ZEFF,
	ACH_KILL_KHAJAG,
	ACH_KILL_ELYSIA,
	ACH_KILL_JANUS,
	ACH_KILL_MERCENARIES,
	ACH_FRIEND_YASHA,
	ACH_KILL_YASHA,
	ACH_KILL_ROYAL_GUARDS,
	ACH_KILL_VELIUS,

	// guilds
	ACH_CLERIC_REP_70,
	ACH_NECRO_REP_70,
	ACH_THIEF_REP_70,

	// general
	ACH_NO_DEATH,
	ACH_GOLD_1000,
	ACH_ALL_MODIFIERS,
	ACH_ALL_BOOKS,
	ACH_ALL_WAYPOINTS,
	ACH_ALL_KEYS,

	// etoz' tower
	ACH_ETOZ_1,
	ACH_ETOZ_2,
	ACH_ETOZ_3,
	ACH_ETOZ_4,

	// other
	ACH_LEEROY,
	ACH_TEARSTAFF,
	ACH_CANADIAN_LOOP,
	ACH_CHICKEN_SHOCKER,
	ACH_NECRONOMICON,
	ACH_CRIMINAL,
	ACH_PRISON_BREAK,
	ACH_MASOCHIST,
	ACH_MINIMALIST,
	ACH_KILL_ALL_MAGES,
	ACH_SAVE_ALL_MAGES,
	ACH_MERCENARY_ORDER,
	ACH_SPEEDRUN,
	ACH_BOOZE,

	MAX
};

static std::map<AchievementID, std::string> AchievementNames = {
	{ ACH_SAVE_ZEFF, "ACH_SAVE_ZEFF" },
	{ ACH_KILL_ZEFF, "ACH_KILL_ZEFF" },
	{ ACH_KILL_KHAJAG, "ACH_KILL_KHAJAG" },
	{ ACH_KILL_ELYSIA, "ACH_KILL_ELYSIA" },
	{ ACH_KILL_JANUS, "ACH_KILL_JANUS" },
	{ ACH_KILL_MERCENARIES, "ACH_KILL_MERCENARIES" },
	{ ACH_FRIEND_YASHA, "ACH_FRIEND_YASHA" },
	{ ACH_KILL_YASHA, "ACH_KILL_YASHA" },
	{ ACH_KILL_ROYAL_GUARDS, "ACH_KILL_ROYAL_GUARDS" },
	{ ACH_KILL_VELIUS, "ACH_KILL_VELIUS" },

	{ ACH_CLERIC_REP_70, "ACH_CLERIC_REP_70" },
	{ ACH_NECRO_REP_70, "ACH_NECRO_REP_70" },
	{ ACH_THIEF_REP_70, "ACH_THIEF_REP_70" },

	{ ACH_NO_DEATH, "ACH_NO_DEATH" },
	{ ACH_GOLD_1000, "ACH_GOLD_1000" },
	{ ACH_ALL_MODIFIERS, "ACH_ALL_MODIFIERS" },
	{ ACH_ALL_BOOKS, "ACH_ALL_BOOKS" },
	{ ACH_ALL_WAYPOINTS, "ACH_ALL_WAYPOINTS" },
	{ ACH_ALL_KEYS, "ACH_ALL_KEYS" },

	{ ACH_ETOZ_1, "ACH_ETOZ_1" },
	{ ACH_ETOZ_2, "ACH_ETOZ_2" },
	{ ACH_ETOZ_3, "ACH_ETOZ_3" },
	{ ACH_ETOZ_4, "ACH_ETOZ_4" },

	{ ACH_LEEROY, "ACH_LEEROY" },
	{ ACH_TEARSTAFF, "ACH_TEARSTAFF" },
	{ ACH_CANADIAN_LOOP, "ACH_CANADIAN_LOOP" },
	{ ACH_CHICKEN_SHOCKER, "ACH_CHICKEN_SHOCKER" },
	{ ACH_NECRONOMICON, "ACH_NECRONOMICON" },
	{ ACH_CRIMINAL, "ACH_CRIMINAL" },
	{ ACH_PRISON_BREAK, "ACH_PRISON_BREAK" },
	{ ACH_MASOCHIST, "ACH_MASOCHIST" },
	{ ACH_MINIMALIST, "ACH_MINIMALIST" },
	{ ACH_KILL_ALL_MAGES, "ACH_KILL_ALL_MAGES" },
	{ ACH_SAVE_ALL_MAGES, "ACH_SAVE_ALL_MAGES" },
	{ ACH_MERCENARY_ORDER, "ACH_MERCENARY_ORDER" },
	{ ACH_SPEEDRUN, "ACH_SPEEDRUN" },
	{ ACH_BOOZE, "ACH_BOOZE" },
};

static std::map<AchievementID, std::string> AchievementGamerzilla = {
	{ ACH_SAVE_ZEFF, "A Wolf in the City" },
	{ ACH_KILL_ZEFF, "You Monster!" },
	{ ACH_KILL_KHAJAG, "The One-Eyed Monster" },
	{ ACH_KILL_ELYSIA, "The End of the Storm" },
	{ ACH_KILL_JANUS, "A Tale of Ice and Fire" },
	{ ACH_KILL_MERCENARIES, "Torches are red, torches are blue..." },
	{ ACH_FRIEND_YASHA, "Demon Friend" },
	{ ACH_KILL_YASHA, "Demon Slayer" },
	{ ACH_KILL_ROYAL_GUARDS, "The Gank Squad" },
	{ ACH_KILL_VELIUS, "Spellbreaker" },

	{ ACH_CLERIC_REP_70, "High Cleric" },
	{ ACH_NECRO_REP_70, "Arch Necromancer" },
	{ ACH_THIEF_REP_70, "Master of Shadows" },

	{ ACH_NO_DEATH, "God" },
	{ ACH_GOLD_1000, "Gold Hoarder" },
	{ ACH_ALL_MODIFIERS, "Gemstone Keeper" },
	{ ACH_ALL_BOOKS, "Literate" },
	{ ACH_ALL_WAYPOINTS, "Globetrotter" },
	{ ACH_ALL_KEYS, "Keymaster" },

	{ ACH_ETOZ_1, "The Novice" },
	{ ACH_ETOZ_2, "The Experienced" },
	{ ACH_ETOZ_3, "The Master" },
	{ ACH_ETOZ_4, "The Extremist" },

	{ ACH_LEEROY, "Leeeeeroy!" },
	{ ACH_TEARSTAFF, "The Sad Fate of Elona" },
	{ ACH_CANADIAN_LOOP, "I'm not your friend, buddy!" },
	{ ACH_CHICKEN_SHOCKER, "Chicken ChaserR" },
	{ ACH_NECRONOMICON, "NecronomiCOME AT ME!" },
	{ ACH_CRIMINAL, "Criminal" },
	{ ACH_PRISON_BREAK, "Prison Break" },
	{ ACH_MASOCHIST, "Masochist" },
	{ ACH_MINIMALIST, "Genius" },
	{ ACH_KILL_ALL_MAGES, "Careless" },
	{ ACH_SAVE_ALL_MAGES, "Leave No One Behind" },
	{ ACH_MERCENARY_ORDER, "A Tough One" },
	{ ACH_SPEEDRUN, "No Time for Dialogues" },
	{ ACH_BOOZE, "12 Step Program" },
};

static std::string getAchievementName(AchievementID id) {
	auto it = AchievementNames.find(id);
	if (it == AchievementNames.end()) return "";

	return (*it).second;
}

static std::string getAchievementGamerzilla(AchievementID id) {
	auto it = AchievementGamerzilla.find(id);
	if (it == AchievementGamerzilla.end()) return "";

	return (*it).second;
}

static AchievementData* createAchievementsArray() {
	int achievementCount = static_cast<int>(MAX) - 1;
	AchievementData* data = new AchievementData[achievementCount];

	for (int i = 0; i < achievementCount; ++i) {
		auto achId = static_cast<AchievementID>(i + 1);
		data[i] = AchievementData();
		data[i].m_achievementIdInt = achId;
		data[i].m_AchievementIdString = getAchievementName(achId).c_str();
	}

	return data;
}
