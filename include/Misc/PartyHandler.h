#pragma once

#include "global.h"
#include "sqlite/sqlite3.h"

// overkill party timed feature

enum PartyScore {
	ENEMY_DEFEATED_FIRST,
	ENEMY_DEFEATED,
	BOSS_DEFEATED,
	ITEM_LOOTED,
	MODIFIER_LEARNED,
	SPELL_LEARNED,
	QUEST_COMPLETED,
	QUEST_FAILED,
	REPUTATION_ADDED,
	ITEM_USED,
	CONDITION_ADDED,
	QUEST_DESCRIPTION_ADDED,
	DEATH,
	PERMANENT_ATTRIBUTES,
	CHEST_LOOTED
};

struct PartyData {
	bool isPartyActive = false;
	int currentID = -1;
	int score = 0;
	sf::Time timeLeft;
	sf::String name;
};

class PartyHandler {
public:
	PartyHandler();
	~PartyHandler();

	// returns the name of the user if the given ID is still available
	// returns an empty string if not.
	std::string checkID(int userID);
	// starts the timer for a new user
	// assert that the old data has been written down.
	void startNewSession(int userID, const std::string& userName);
	void endCurrentSession();
	void notifyPartyScore(PartyScore score);

	// add file io
	// add db managing (only read)

	PartyData& getData() { return m_data; }

private:
	void initDatabase();

private:
	PartyData m_data;
	sqlite3 *m_db = nullptr;

	static const sf::Time PARTY_TIME;
	static const std::string OUTPUT_PATH;
	static const std::string DATABASE_PATH;
};