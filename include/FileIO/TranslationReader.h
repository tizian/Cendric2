#pragma once

#include "global.h"
#include "FileIO/CSVReader.h"

#include "Enums/Language.h"

// reads translations from a specified csv file
// used by the text provider.
class TranslationReader : public CSVReader
{
public:
	TranslationReader();
	~TranslationReader();

	// fills the translation map with the translations of the language specified.
	bool readTranslations(Language lang, std::map<std::string, std::wstring>& translationMap, const std::string& filename) const;
};