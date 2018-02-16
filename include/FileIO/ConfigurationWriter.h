#pragma once

#include <fstream>
#include <sstream>

#include "global.h"
#include "Structs/ConfigurationData.h"
#include "FileIO/ConfigurationIO.h"

// a writer to write to the configuration file
class ConfigurationWriter final : public ConfigurationIO {
public:
	bool saveToFile(const ConfigurationData& data) const;

private:
	std::string writeConfigurationHeader() const;
	std::string writeSoundOn(const ConfigurationData& data) const;
	std::string writeSoundVolumeMusic(const ConfigurationData& data) const;
	std::string writeSoundVolumeSound(const ConfigurationData& data) const;
	std::string writeFPSMax(const ConfigurationData& data) const;
	std::string writeFPSLimitOn(const ConfigurationData& data) const;
	std::string writeQuickcastOn(const ConfigurationData& data) const;
	std::string writeAutotargetOn(const ConfigurationData& data) const;
	std::string writeHintsOn(const ConfigurationData& data) const;
	std::string writeQuestmarkersOn(const ConfigurationData& data) const;

	std::string writeLanguage(const ConfigurationData& data) const;
	std::string writeDisplayMode(const ConfigurationData& data) const;
	std::string writeMainInputMap(const ConfigurationData& data) const;
	std::string writeAlternativeInputMap(const ConfigurationData& data) const;
	std::string writeVSyncOn(const ConfigurationData& data) const;

	std::string writeMultithreadingOn(const ConfigurationData& data) const;
	std::string writeDebugRenderingOn(const ConfigurationData& data) const;
	std::string writeSmoothingOn(const ConfigurationData& data) const;
	std::string writeLogLevel(const ConfigurationData& data) const;
	std::string writeIsDisplayTime(const ConfigurationData& data) const;
};