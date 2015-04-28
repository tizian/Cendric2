#pragma once

#include "global.h"

#include <map>

enum class Texts
{
	Play,
	Exit,
	Credits
};

enum class Language
{
	Lang_EN,
	Lang_DE
};

class TextProvider
{
public:
	// inits the text vectors
	void init();
	std::string getText(Texts key);
	void setLanguage(Language lang);

private:
	void addText(Texts key, std::string english, std::string german);
	std::map<Texts, std::string> m_germanTexts;
	std::map<Texts, std::string> m_englishTexts;
	Language m_language = Language::Lang_EN;
};