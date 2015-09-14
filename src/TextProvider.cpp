#include "TextProvider.h"
#include "ResourceManager.h"

using namespace std;

TextProvider* g_textProvider;

void TextProvider::loadDialogueText(const std::string& filename)
{
	releaseDialogueText();
	std::map<std::string, std::wstring> dialogueMap;
	TranslationReader reader;
	if (!reader.readTranslations(m_language, dialogueMap, filename))
	{
		g_logger->logInfo("TextProvider", "Dialogue has no translations or file: " + filename + " does not exist.");
		return;
	}
	m_currentDialogue = filename;
	for (auto& it : dialogueMap)
	{
		m_translationMap.insert(it);
		m_currentDialogueTexts.push_back(it.first);
	}
}

void TextProvider::loadQuestText()
{
	releaseQuestText();
	std::map<std::string, std::wstring> questMap;
	TranslationReader reader;
	if (!reader.readTranslations(m_language, questMap, TRANSLATION_QUEST_FILENAME))
	{
		return;
	}
	for (auto& it : questMap)
	{
		m_translationMap.insert(it);
		m_questTexts.push_back(it.first);
	}
}

void TextProvider::releaseQuestText()
{
	for (auto& it : m_questTexts)
	{
		auto& element = m_translationMap.find(it);
		if (element != m_translationMap.end()) m_translationMap.erase(element);
	}
	m_questTexts.clear();
}

void TextProvider::releaseDialogueText()
{
	if (m_currentDialogue.empty()) return;
	for (auto& it : m_currentDialogueTexts)
	{
		auto& element = m_translationMap.find(it);
		if (element != m_translationMap.end()) m_translationMap.erase(element);
	}
	m_currentDialogue = "";
	m_currentDialogueTexts.clear();
}

void TextProvider::reload()
{
	if (m_language != g_resourceManager->getConfiguration().language)
	{
		m_translationMap.clear();
		m_currentDialogue = "";
		m_currentDialogueTexts.clear();
		setLanguage(g_resourceManager->getConfiguration().language);
		TranslationReader reader;
		reader.readTranslations(m_language, m_translationMap, TRANSLATION_FILENAME);
	}
}

const std::wstring& TextProvider::getText(const std::string& key)
{
	if (m_translationMap.find(key) == m_translationMap.end())
	{
		// fallback
		g_logger->logWarning("TranslationReader", "Tried to get missing translation for key: " + key);
		m_fallbackString = L"(undefined text " + std::wstring(key.begin(), key.end()) + L")";
		return m_fallbackString;
	}
	return m_translationMap.at(key);
}

std::wstring TextProvider::getCroppedText(const std::string& key, int characterSize, int maxWidth)
{
	// preconditions
	if (characterSize < 1 || maxWidth < characterSize)
	{
		return L"";
	}

	int maxLineChars = maxWidth / characterSize;
	std::wstring uncroppedText = getText(key);
	std::wstring text = L"";
	while (uncroppedText.size() * characterSize > maxWidth)
	{
		// check for forced newlines
		size_t found = uncroppedText.find(L"\n");
		if (found != string::npos && found < maxLineChars && found++ != string::npos)
		{
			text.append(uncroppedText.substr(0, found));
			uncroppedText = uncroppedText.substr(found);
			continue;
		}
		// check if we need to crop a whole word
		found = uncroppedText.find(L" ");
		if (found == string::npos || (found != string::npos && found > maxLineChars))
		{
			text.append(uncroppedText.substr(0, maxLineChars));
			text.append(L"\n");
			uncroppedText = uncroppedText.substr(maxLineChars);
			continue;
		}
		// append words as long as we have still space for them
		int space = maxLineChars;
		while (true)
		{
			found = uncroppedText.find(L" ");
			if (found != string::npos && found++ < space)
			{
				text.append(uncroppedText.substr(0, found));
				uncroppedText = uncroppedText.substr(found);
				space = space - static_cast<int>(found);
			}
			else
			{
				text.append(L"\n");
				break;
			}
		}
	}
	return text.append(uncroppedText);

}

void TextProvider::setLanguage(Language lang)
{
	m_language = lang;
}

