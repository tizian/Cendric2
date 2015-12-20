#pragma once

#include "global.h"
#include "Logger.h"
#include "Enums/Language.h"
#include "FileIO/TranslationReader.h"

#include <map>

class TextProvider {
public:
	std::string getText(const std::string& key);
	// adds newline characters to the string where needed. Don't call this too often as
	// it calculates the newline characters new every time you call it.
	std::string getCroppedText(const std::string& key, int characterSize, int maxWidth);
	void setLanguage(Language lang);
	void reload();

private:
	std::string m_language = "english";
};