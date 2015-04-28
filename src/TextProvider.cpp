#include "TextProvider.h"

using namespace std;

TextProvider* g_textProvider;

void TextProvider::init()
{
	addText(Texts::Play, "Play", "Spielen");
	addText(Texts::Credits, "Credits", "Credits");
	addText(Texts::Exit, "Exit", "Beenden");
}

std::string TextProvider::getText(Texts key)
{
	switch (m_language)
	{
	case Language::Lang_DE:
		return m_germanTexts[key];
	case Language::Lang_EN:
		return m_englishTexts[key];
	default:
		// unexpected
		return m_englishTexts[key];;
	}
}

void TextProvider::setLanguage(Language lang)
{
	m_language = lang;
}

void TextProvider::addText(Texts key, std::string english, std::string german)
{
	m_englishTexts.insert({ key, english });
	m_germanTexts.insert({ key, german });
}


