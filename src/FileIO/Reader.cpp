#include "FileIO/Reader.h"
#include <sstream>

bool Reader::readPositiveInt(const std::string& s, int* i) const {
	*i = std::stoi(s);
	if (*i < 0) {
		*i = 0;
		return false;
	}
	return true;
}

bool Reader::readIntAfterComma(std::string& s, int* i) const {
	std::size_t pos = s.find(',');

	if (pos == std::string::npos) return false;
	if (s.size() == 1) return false;

	s = s.substr(pos + 1);
	*i = std::stoi(s);

	return true;
}

bool Reader::readFloatAfterComma(std::string& s, float* f) const {
	std::size_t pos = s.find(',');

	if (pos == std::string::npos) return false;
	if (s.size() == 1) return false;

	s = s.substr(pos + 1);
	*f = std::stof(s);

	return true;
}

void Reader::replace(std::string& str, const std::string& oldStr, const std::string& newStr) const {
	size_t pos = 0;
	while ((pos = str.find(oldStr, pos)) != std::string::npos) {
		str.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
}

