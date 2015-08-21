#pragma once

#include <fstream>
#include <sstream>

#include "global.h"
#include "Logger.h"

#include "Structs/ConfigurationData.h"
#include "FileIO/ConfigurationIO.h"

// a writer to write to the configuration file
class ConfigurationWriter : public ConfigurationIO
{
public:
	bool saveToFile(const ConfigurationData& data) const;

private:
	std::string writeConfigurationHeader() const;
	std::string writeSoundOn(const ConfigurationData& data) const;
	std::string writeSoundVolume(const ConfigurationData& data) const;
	std::string writeQuickcastOn(const ConfigurationData& data) const;

	std::string writeLanguage(const ConfigurationData& data) const;
	std::string writeMainInputMap(const ConfigurationData& data) const;
	std::string writeAlternativeInputMap(const ConfigurationData& data) const;
	std::string writeMaxFPS(const ConfigurationData& data) const;

	std::string writeDebugModeOn(const ConfigurationData& data) const;
};