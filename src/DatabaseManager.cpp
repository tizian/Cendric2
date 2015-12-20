#include "DatabaseManager.h"

DatabaseManager::~DatabaseManager() {
	sqlite3_close(m_db);
}

void DatabaseManager::init() {
	int rc = sqlite3_open(DB_FILENAME.c_str(), &m_db);

	if (rc) {
		g_logger->logError("DatabaseManager", "Can't open database:" + std::string(sqlite3_errmsg(m_db)));
		return;
	}

	g_logger->logInfo("DatabaseManager", "Opened database successfully");
}

ResultSet DatabaseManager::query(const std::string& query_) const {
	sqlite3_stmt *statement;
	ResultSet results;

	if (sqlite3_prepare_v2(m_db, query_.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		int result = 0;
		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				std::vector<std::string> values;
				for (int col = 0; col < cols; col++) {
					values.push_back((char*)sqlite3_column_text(statement, col));
				}
				results.push_back(values);
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

	return results;
}

