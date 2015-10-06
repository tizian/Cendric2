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

std::wstring Reader::getFileContentsWide(const char *filename) const {
	std::wifstream in(filename, std::ios::in);
	if (in) {
		std::wstring contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	else {
		g_logger->logError("Reader", "Unable to read file: " + std::string(filename));
		return L"";
	}
}

std::string Reader::getFileContents(const char *filename) const {
	std::ifstream in(filename, std::ios::in);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	else {
		g_logger->logError("Reader", "Unable to read file: " + std::string(filename));
		return "";
	}
}