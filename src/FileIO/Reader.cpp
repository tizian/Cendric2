#include "FileIO/Reader.h"
#include <sstream>

char* Reader::gotoNextChar(char* buffer, char* end, char goal) const {
	while (buffer < end && *buffer != goal) {
		buffer++;
	}

	// goal not found
	if (buffer >= end) {
		return nullptr;
	}

	return buffer;
}

int Reader::countToNextChar(char* buffer, char* end, char goal) const {
	int count = 0;
	while (buffer < end && *buffer != goal) {
		buffer++;
		count++;
	}

	// goal not found
	if (buffer >= end) {
		return -1;
	}

	return count;
}

inline void replace(std::string& str, const std::string& oldStr, const std::string& newStr) {
	size_t pos = 0;
	while ((pos = str.find(oldStr, pos)) != std::string::npos) {
		str.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
}

std::string Reader::getFileContents(const char *filename) const {
	std::ifstream in(filename, std::ios::binary);
	std::string res = "";
	if (in) {
		in.seekg(0, std::ios::end);
		const int size = static_cast<int>(in.tellg());
		char* content = new char[size + 1];
		in.seekg(0, std::ios::beg);
		in.read(content, size - 1);

		// don't forget to null terminate char sequence
		content[size] = '\0';

		// parse to string and remove \r line endings
		std::string s = std::string(content);
		s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());

 #ifdef __linux__
		// ä ö ü conversion for beloved linux that interprets our ansi as utf-8
		for (int i = 0; i < s.size(); i++) {
			if (s[i] == -28) {
				res.append("ae");
			}
			else if (s[i] == -10) {
				res.append("oe");
			}
			else if (s[i] == -4) {
				res.append("ue");
			}
			else if (s[i] == -60) {
				res.append("Ae");
			}
			else if (s[i] == -42) {
				res.append("Oe");
			}
			else if (s[i] == -36) {
				res.append("Ue");
			}
			else {
				res += s[i];
			}
		}

		// remove duplicate ueue etc as it looks weird
		replace(res, "ueue", "ue");
		replace(res, "oeoe", "oe");
		replace(res, "aeae", "ae");

#else
		res = s;
#endif
		delete[] content;
		in.close();
	}
	else {
		g_logger->logError("Reader", "Unable to read file: " + std::string(filename));
	}
	return res;
}
