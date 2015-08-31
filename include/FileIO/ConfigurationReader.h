#pragma once

#include <set>

#include "global.h"
#include "FileIO/Reader.h"
#include "FileIO/ConfigurationIO.h"
#include "Logger.h"

#include "Structs/ConfigurationData.h"

// a reader to read a configuration file
class ConfigurationReader : public Reader, public ConfigurationIO
{
public:
	bool readConfiguration(ConfigurationData& data) const;
	// checks a configuration, namely the key map, for duplicate entries.
	bool checkConfigurationData(ConfigurationData& data) const;

private:
	bool readLanguage(const std::string& line, ConfigurationData& data) const;
	bool readMaxFPS(const std::string& line, ConfigurationData& data) const;
	bool readMainInputMapping(const std::string& line, ConfigurationData& data) const;
	bool readAlternativeInputMapping(const std::string& line, ConfigurationData& data) const;

	bool readSoundVolume(const std::string& line, ConfigurationData& data) const;
	bool readSoundOn(const std::string& line, ConfigurationData& data) const;

	bool readQuickcastOn(const std::string& line, ConfigurationData& data) const;
	bool readDebugModeOn(const std::string& line, ConfigurationData& data) const;
	bool readDebugRenderingOn(const std::string& line, ConfigurationData& data) const;
};