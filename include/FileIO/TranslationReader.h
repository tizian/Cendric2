#pragma once

#include <sstream>

#include "global.h"
#include "FileIO/Reader.h"
#include "Logger.h"
#include "ResourceManager.h"

#include "Enums/Language.h"

typedef std::vector<std::vector<std::wstring> > StringTable;

// reads translations from a specified csv file
// used by the text provider.
class TranslationReader : public Reader
{
public:
	TranslationReader();
	~TranslationReader();

	// clears the translation map and fills it again with the translations of the language specified.
	// returns if the operation was successful
	bool readTranslations(Language lang, std::map<std::string, std::wstring>& translationMap) const;

private: 
	void parseCsv(wchar_t *csvData, StringTable& table) const;
	wchar_t* nextCsvField(wchar_t *p, bool *newline, wchar_t **escapedEnd) const;
	
};