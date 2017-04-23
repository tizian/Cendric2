#pragma once

#include <fstream>

#include "global.h"
#include "Logger.h"

class Reader {
public: 
	virtual ~Reader() {}
protected:
	const char COMMENT_MARKER = '#';

	bool readPositiveInt(const std::string& s, int* i) const;
	bool readIntAfterComma(std::string& s, int* i) const;
	bool readFloatAfterComma(std::string& s, float* f) const;
	void replace(std::string& str, const std::string& oldStr, const std::string& newStr) const;
};