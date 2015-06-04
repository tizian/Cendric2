#pragma once

#include "global.h"
#include "Enums/Language.h"
#include "FileIO/TranslationReader.h"

#include <map>

class TextProvider
{
public:
	// clears the map & loads the translations using the current language
	void reload();
	const std::wstring& getText(const std::string& key);
	void setLanguage(Language lang);

private:
	Language m_language = Language::Lang_DE;
	std::map<std::string, std::wstring> m_translationMap;
	std::wstring m_fallbackString = L"{undefined text}";
};