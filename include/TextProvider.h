#pragma once

#include "global.h"
#include "Logger.h"
#include "Enums/Language.h"
#include "FileIO/TranslationReader.h"

#include <map>

class TextProvider
{
public:
	// clears the map & loads the translations using the current language
	void reload();
	const std::wstring& getText(const std::string& key);
	// adds newline characters to the string where needed. Don't call this too often as
	// it calculates the newline characters new every time you call it.
	std::wstring getCroppedText(const std::string& key, int characterSize, int maxWidth);
	void setLanguage(Language lang);

private:
	Language m_language = Language::Lang_DE;
	std::map<std::string, std::wstring> m_translationMap;
	std::wstring m_fallbackString = L"{undefined text}";
};