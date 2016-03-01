#pragma once

#include "global.h"
#include "Logger.h"
#include "Enums/Language.h"

#include <map>

class TextProvider {
public:
	// this assumes a text of type "core"
	std::string getText(const std::string& key);
	std::string getText(const std::string& key, const std::string& type);
	// adds newline characters to the string where needed. Don't call this too often as
	// it calculates the newline characters new every time you call it. It assumes a text of type "core"
	std::string getCroppedText(const std::string& key, int characterSize, int maxWidth);
	std::string getCroppedText(const std::string& key, const std::string& type, int characterSize, int maxWidth);
	std::string getCroppedString(const std::string& text, int characterSize, int maxWidth);
	void setLanguage(Language lang);
	void reload();

private:
	std::string m_language = "english";
};