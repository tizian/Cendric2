#pragma once

#include "global.h"
#include "Enums/Language.h"
#include "Enums/Texts.h"

#include <map>

class TextProvider
{
public:
	// inits the text vectors
	void init();
	const std::string& getText(Texts key);
	void setLanguage(Language lang);

private:
	void addText(Texts key, const std::string& english, const std::string& german);
	std::map<Texts, std::string> m_germanTexts;
	std::map<Texts, std::string> m_englishTexts;
	Language m_language = Language::Lang_DE;
};