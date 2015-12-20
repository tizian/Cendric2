#pragma once


#include "global.h"
#include "Logger.h"

#include "sqlite/sqlite3.h"

typedef std::vector<std::vector<std::string>> ResultSet;

/* interface to the SQLite database */
class DatabaseManager {
public:
	DatabaseManager() {};
	~DatabaseManager();

	void init();
	ResultSet query(const std::string& query) const;

private:
	const std::string DB_FILENAME = "db/cendricdb.db";

	sqlite3 *m_db;
};