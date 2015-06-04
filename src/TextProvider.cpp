#include "TextProvider.h"

using namespace std;

TextProvider* g_textProvider;

void TextProvider::reload()
{
	TranslationReader reader;
	reader.readTranslations(m_language, m_translationMap);
}

const std::wstring& TextProvider::getText(const std::string& key) const
{
	if (m_translationMap.find(key) == m_translationMap.end())
	{
		// fallback
		g_logger->logWarning("TranslationReader", "Tried to get missing translation for key: " + key);
		return L"{undefined text " + std::wstring(key.begin(), key.end()) + L"}";
	}
	return m_translationMap.at(key);
}

void TextProvider::setLanguage(Language lang)
{
	m_language = lang;
}

