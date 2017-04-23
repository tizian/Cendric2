#pragma once

#include "global.h"

// static class to load and save the configuration file
class ConfigurationIO final {
public:
	// returns whether the operation was successful
	static bool loadConfiguration();
	// returns whether the operation was successful
	static bool saveConfiguration();

private:
	static bool readPath(std::string& path, const std::string& line);
	static std::string writeConfigurationHeader();
	static std::string writePath(const std::string& path, const std::string& tag);

private:
	static const std::string CONFIGURATION_PATH;

	static const std::string SQL_PATH;
	static const std::string NPC_PATH;
	static const std::string DIALOGUE_PATH;
	static const std::string PARSE_IN;
	static const std::string PARSE_OUT;
};