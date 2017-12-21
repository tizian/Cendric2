#pragma once

#include "global.h"
#include "FileIO/Reader.h"
#include "FileIO/ConfigurationIO.h"
#include "Logger.h"

#include "Structs/ConfigurationData.h"

// a reader to read a configuration file
class ConfigurationReader final : public Reader, public ConfigurationIO {
public:
	bool readConfiguration(ConfigurationData& data) const;
	// checks a configuration, namely the key map, for duplicate entries.
	bool checkConfigurationData(ConfigurationData& data) const;

private:
	bool readLanguage(const std::string& line, ConfigurationData& data) const;
	bool readDisplayMode(const std::string& line, ConfigurationData& data) const;
	bool readMainInputMapping(const std::string& line, ConfigurationData& data) const;
	bool readAlternativeInputMapping(const std::string& line, ConfigurationData& data) const;

	bool readSoundVolumeMusic(const std::string& line, ConfigurationData& data) const;
	bool readSoundVolumeSound(const std::string& line, ConfigurationData& data) const;
	bool readSoundOn(const std::string& line, ConfigurationData& data) const;
	bool readVSyncOn(const std::string& line, ConfigurationData& data) const;
	bool readFPSLimitOn(const std::string& line, ConfigurationData& data) const;
	bool readFPSMax(const std::string& line, ConfigurationData& data) const;

	bool readQuickcastOn(const std::string& line, ConfigurationData& data) const;
	bool readHintsOn(const std::string& line, ConfigurationData& data) const;
	bool readQuestmarkersOn(const std::string& line, ConfigurationData& data) const;

	bool readStopwatchOn(const std::string& line, ConfigurationData& data) const;
	bool readDebugRenderingOn(const std::string& line, ConfigurationData& data) const;
	bool readSmoothingOn(const std::string& line, ConfigurationData& data) const;
	bool readAutotargetOn(const std::string& line, ConfigurationData& data) const;

private:
	bool readBoolean(const std::string& line, bool& data) const;
};