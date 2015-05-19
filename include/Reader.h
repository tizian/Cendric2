#pragma once

#include "global.h"

class Reader
{
public:
	
protected:
	const char COMMENT_MARKER = '#';

	char* gotoNextChar(char* buffer, char* end, char goal) const;
	int countToNextChar(char* buffer, char* end, char goal) const;
};