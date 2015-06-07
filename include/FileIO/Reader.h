#pragma once

#include <fstream>

#include "global.h"
#include "Logger.h"

class Reader
{
public:
	
protected:
	const char COMMENT_MARKER = '#';

	char* gotoNextChar(char* buffer, char* end, char goal) const;
	int countToNextChar(char* buffer, char* end, char goal) const;
	std::wstring getFileContentsWide(const char *filename) const;
	std::string getFileContents(const char *filename) const;
};