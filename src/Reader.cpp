#include "Reader.h"

char* Reader::gotoNextChar(char* buffer, char* end, char goal) const
{
	while (buffer < end && *buffer != goal)
	{
		buffer++;
	}

	// goal not found
	if (buffer >= end)
	{
		return NULL;
	}

	return buffer;
}

int Reader::countToNextChar(char* buffer, char* end, char goal) const
{
	int count = 0;
	while (buffer < end && *buffer != goal)
	{
		buffer++;
		count++;
	}

	// goal not found
	if (buffer >= end)
	{
		return -1;
	}

	return count;
}