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

	void loadDialogueText(const std::string& filename);
	void releaseDialogueText();

private:
	const std::string TRANSLATION_CORE_FILENAME = "res/translations_core.csv";
	const std::string TRANSLATION_QUESTS_FILENAME = "res/translations_quests.csv";
	const std::string TRANSLATION_ITEMS_FILENAME = "res/translations_items.csv";

	std::string m_currentDialogue = "";
	std::vector<std::string> m_currentDialogueTexts;

	Language m_language = Language::VOID;
	std::map<std::string, std::wstring> m_translationMap;
	std::wstring m_fallbackString = L"{undefined text}";
};