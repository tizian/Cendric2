#include <fstream>

#include "Misc/PartyHandler.h"
#include "Logger.h"

const sf::Time PartyHandler::PARTY_TIME = sf::seconds(6 * 60);
const std::string PartyHandler::OUTPUT_PATH = "party/cendric.txt";
const std::string PartyHandler::DATABASE_PATH = "party/db/leaderboard.db";

PartyHandler::PartyHandler() {
	initDatabase();
}

PartyHandler::~PartyHandler() {
	if (m_db != nullptr)
		sqlite3_close(m_db);
}

std::string PartyHandler::checkID(int userID) {
	if (m_db == nullptr) return "";

	if (userID == 1337) {
		return "Ironbell";
	}

	std::string userName = "";
	sqlite3_stmt* statement;

	std::string query = "SELECT name FROM users WHERE NOT(cendric_locked) AND user_id = " + std::to_string(userID) + ";";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		int result = 0;
		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				userName = std::string((char*)sqlite3_column_text(statement, 0));
			}
			else {
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	std::string error = sqlite3_errmsg(m_db);
	if (error.compare("not an error") != 0) {
		g_logger->logError("DatabaseManager", error);
	}

	return userName;
}

void PartyHandler::startNewSession(int userID, const std::string& userName) {
	if (m_data.isPartyActive) {
		g_logger->logError("PartyHandler", "Can't start new session, old one is still running!");
		return;
	}

	m_data.currentID = userID;
	m_data.name = userName;
	m_data.timeLeft = PARTY_TIME;
	m_data.score = 0;
	m_data.isPartyActive = true;
}

void PartyHandler::initDatabase() {
	int rc = sqlite3_open(getResourcePath(DATABASE_PATH).c_str(), &m_db);

	if (rc) {
		g_logger->logError("PartyHandler", "Can't open database:" + std::string(sqlite3_errmsg(m_db)));
		return;
	}

	g_logger->logInfo("PartyHandler", "Opened database successfully");
}

void PartyHandler::endCurrentSession() {
	if (!m_data.isPartyActive) {
		g_logger->logError("PartyHandler", "No session to end.");
		return;
	}

	if (m_data.currentID == 1337) {
		// won't write that down.
		m_data.currentID = -1;
		return;
	}

	std::ofstream savefile(OUTPUT_PATH, std::ios::trunc);
	if (savefile.is_open()) {
		savefile << std::to_string(m_data.currentID) + "\n";
		savefile << std::to_string(m_data.score);

		savefile.close();
	}
	else {
		g_logger->logError("PartyHandler", "Unable to open file: " + std::string(OUTPUT_PATH));
	}

	m_data.isPartyActive = false;
}

void PartyHandler::notifyPartyScore(PartyScore score) {
	switch (score) {
	default: 
		break;
	case PartyScore::BOSS_DEFEATED:
		m_data.score += 30;
		break;
	case PartyScore::CONDITION_ADDED:
		m_data.score += 2;
		break;
	case PartyScore::DEATH:
		m_data.score -= 5;
		break;
	case PartyScore::ENEMY_DEFEATED:
		m_data.score += 2;
		break;
	case PartyScore::ENEMY_DEFEATED_FIRST:
		m_data.score += 3;
		break;
	case PartyScore::ITEM_LOOTED:
		m_data.score += 1;
		break;
	case PartyScore::ITEM_USED:
		m_data.score -= 1;
		break;
	case PartyScore::MODIFIER_LEARNED:
		m_data.score += 20;
		break;
	case PartyScore::QUEST_COMPLETED:
		m_data.score += 10;
		break;
	case PartyScore::QUEST_FAILED:
		m_data.score -= 5;
		break;
	case PartyScore::QUEST_DESCRIPTION_ADDED:
		m_data.score += 2;
		break;
	case PartyScore::REPUTATION_ADDED:
		m_data.score += 5;
		break;
	case PartyScore::SPELL_LEARNED:
		m_data.score += 10;
		break;
	case PartyScore::PERMANENT_ATTRIBUTES:
		m_data.score += 1;
		break;
	case PartyScore::CHEST_LOOTED:
		m_data.score += 5;
		break;
	case PartyScore::YOU_MONSTER:
		m_data.score -= 5;
		break;
	}

	if (m_data.score < 0) {
		m_data.score = 0;
	}
}

