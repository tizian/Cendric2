#pragma once

#include <fstream>

#include "global.h"
#include "Logger.h"

class Reader {
public:

protected:
	const char COMMENT_MARKER = '#';

	char* gotoNextChar(char* buffer, char* end, char goal) const;
	char* gotoNextChar(char* buffer, char* end) const;
	int countToNextChar(char* buffer, char* end, char goal) const;
	std::string getFileContents(const char *filename) const;
};