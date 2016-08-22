#pragma once

#include "global.h"

struct TranslationData {
	std::string tag;
	std::string englishTranslation;
};

// a class that parses cendric insert scripts to a format our translater uses
// and back
class InsertScriptParser {
public:
	InsertScriptParser();
	~InsertScriptParser();

	void showParseMenu();
	void parseInsertScript() const;

private:
	std::string writeParserHeader() const;

	char m_inputPath[100];
	char m_outputPath[100];
};