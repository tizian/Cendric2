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

void PartyHandler::startNewSession(int userID, const std::string& userName) {
	if (m_data.currentID != -1) {
		g_logger->logError("PartyHandler", "Can't start new session, old one is still running!");
		return;
	}

	m_data.currentID = userID;
	m_data.name = userName;
	m_data.timeLeft = PARTY_TIME;
	m_data.score = 0;
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
	if (m_data.currentID == -1) {
		g_logger->logError("PartyHandler", "No session to end.");
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

	m_data.currentID = -1;
}

