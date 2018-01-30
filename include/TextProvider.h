#pragma once

#include "global.h"
#include "Logger.h"
#include "Enums/Language.h"

class TextProvider final {
public:
	TextProvider();

	// this assumes a text of type "core"
	std::string getText(const std::string& key);
	std::string getText(const std::string& key, const std::string& type, bool isReplaceItemVariables = true, bool isRaw = false);
	
	// adds newline characters to the string where needed. Don't call this too often as
	// it calculates the newline characters new every time you call it. It assumes a text of type "core"
	std::string getCroppedText(const std::string& key, int characterSize, int maxWidth, bool isRaw = false);
	std::string getCroppedText(const std::string& key, const std::string& type, int characterSize, int maxWidth, bool isRaw = false);
	std::string getCroppedString(const std::string& text, int characterSize, int maxWidth);

	bool isTextTranslated(const std::string& key, const std::string& type);
	
	void setLanguage(Language lang);
	void reload();

private:
	std::string m_language = "english";
	// replaces the item variables marked with $item_id$ in the text
	void replaceItemVariables(std::string& text);
	// transforms the special characters of a string to characters that can be used in the bitmap text
	static std::string transform(const std::string& in);

	static const int TAB_TO_SPACES;
};