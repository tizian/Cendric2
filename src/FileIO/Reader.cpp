#include "FileIO/Reader.h"

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

std::string Reader::getFileContents(const char *filename) const {
	std::ifstream in(filename, std::ios::binary);
	std::string s = "";
	if (in) {
		in.seekg(0, std::ios::end);
		const int size = static_cast<int>(in.tellg());
		char* content = new char[size];
		in.seekg(0, std::ios::beg);
		in.read(content, size);
		// don't forget to null terminate char sequence
		content[size - 1] = '\0';
		
		// parse to string and remove \r line endings
		s = std::string(content);
		s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
		delete[] content;
		in.close();
	}
	else {
		g_logger->logError("Reader", "Unable to read file: " + std::string(filename));
	}
	return s;
}
