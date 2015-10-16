#pragma once

#include <sstream>

#include "global.h"
#include "FileIO/Reader.h"
#include "Logger.h"

#include "Enums/Language.h"

typedef std::vector<std::vector<std::string>> StringTable;

// reads columns from a specified csv file
class CSVReader : public Reader {
protected:
	void parseCsv(const char *csvData, StringTable& table) const;
	const char* nextCsvField(const char *p, bool *newline, const char **escapedEnd) const;
};