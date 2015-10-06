#pragma once

#include <sstream>

#include "global.h"
#include "FileIO/Reader.h"
#include "Logger.h"

#include "Enums/Language.h"

typedef std::vector<std::vector<std::wstring> > StringTable;

// reads columns from a specified csv file
class CSVReader : public Reader {
protected:
	void parseCsv(wchar_t *csvData, StringTable& table) const;
	wchar_t* nextCsvField(wchar_t *p, bool *newline, wchar_t **escapedEnd) const;
};