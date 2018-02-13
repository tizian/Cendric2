#include "FileIO/ConfigurationReader.h"
#include "GlobalResource.h"

#include <cstring>

bool ConfigurationReader::readConfiguration(ConfigurationData& data, bool retry) const {
	data.resetToDefault();

	std::string line;
	std::ifstream configuration(getDocumentsPath(GlobalResource::CONFIGURATION_PATH));
	bool noError = true;
	if (configuration.is_open()) {
		while (getline(configuration, line)) {
			if (line.empty() || line.compare(0, 1, "#") == 0) {
				continue;
			}
			else if (line.compare(0, strlen(LANGUAGE), std::string(LANGUAGE)) == 0) {
				noError = readLanguage(line, data);
			}
			else if (line.compare(0, strlen(MAIN_INPUT_MAPPING), std::string(MAIN_INPUT_MAPPING)) == 0) {
				noError = readMainInputMapping(line, data);
			}
			else if (line.compare(0, strlen(ALTERNATIVE_INPUT_MAPPING), std::string(ALTERNATIVE_INPUT_MAPPING)) == 0) {
				noError = readAlternativeInputMapping(line, data);
			}
			else if (line.compare(0, strlen(VSYNC_ON), std::string(VSYNC_ON)) == 0) {
				noError = readVSyncOn(line, data);
			}
			else if (line.compare(0, strlen(FPS_LIMIT_ON), std::string(FPS_LIMIT_ON)) == 0) {
				noError = readFPSLimitOn(line, data);
			}
			else if (line.compare(0, strlen(FPS_MAX), std::string(FPS_MAX)) == 0) {
				noError = readFPSMax(line, data);
			}
			else if (line.compare(0, strlen(SOUND_ON), std::string(SOUND_ON)) == 0) {
				noError = readSoundOn(line, data);
			}
			else if (line.compare(0, strlen(SOUND_VOLUME_MUSIC), std::string(SOUND_VOLUME_MUSIC)) == 0) {
				noError = readSoundVolumeMusic(line, data);
			}
			else if (line.compare(0, strlen(SOUND_VOLUME_SOUND), std::string(SOUND_VOLUME_SOUND)) == 0) {
				noError = readSoundVolumeSound(line, data);
			}
			else if (line.compare(0, strlen(QUICKCAST_ON), std::string(QUICKCAST_ON)) == 0) {
				noError = readQuickcastOn(line, data);
			}
			else if (line.compare(0, strlen(HINTS_ON), std::string(HINTS_ON)) == 0) {
				noError = readHintsOn(line, data);
			}
			else if (line.compare(0, strlen(QUESTMARKERS_ON), std::string(QUESTMARKERS_ON)) == 0) {
				noError = readQuestmarkersOn(line, data);
			}
			else if (line.compare(0, strlen(MULTITHREADING_ON), std::string(MULTITHREADING_ON)) == 0) {
				noError = readMultithreadingOn(line, data);
			}
			else if (line.compare(0, strlen(DEBUGRENDERING_ON), std::string(DEBUGRENDERING_ON)) == 0) {
				noError = readDebugRenderingOn(line, data);
			}
			else if (line.compare(0, strlen(SMOOTHING_ON), std::string(SMOOTHING_ON)) == 0) {
				noError = readSmoothingOn(line, data);
			}
			else if (line.compare(0, strlen(AUTOTARGET_ON), std::string(AUTOTARGET_ON)) == 0) {
				noError = readAutotargetOn(line, data);
			}
			else if (line.compare(0, strlen(DISPLAYMODE), std::string(DISPLAYMODE)) == 0) {
				noError = readDisplayMode(line, data);
			}
			else {
				g_logger->logError("ConfigurationReader", "Unknown tag found in configuration file on line: " + line);
				noError = false;
			}
			if (!noError) {
				break;
			}
		}
		configuration.close();
		if (!noError || !checkConfigurationData(data)) {
			return false;
		}
	}
	else {
		if (!retry) {
			g_logger->logWarning("ConfigurationReader", "Unable to open configuration file. Default configuration is used.");
			return false;
		}

		auto externalPath = getDocumentsPath(GlobalResource::CONFIGURATION_PATH);
		auto internalPath = GlobalResource::CONFIGURATION_PATH;
			
		if (externalPath != internalPath) {
			// try to copy config file over

			std::ifstream  src(internalPath, std::ios::binary);
			std::ofstream  dst(externalPath, std::ios::binary);

			if (src.is_open() && dst.is_open()) {
				dst << src.rdbuf();
				g_logger->logInfo("ConfigurationReader", "Copying configuration file to external location.");
				src.close();
				dst.close();
				return readConfiguration(data, false);
			}
		}

		g_logger->logWarning("ConfigurationReader", "Unable to open configuration file. Default configuration is used.");
		return false;
	}

	return true;
}

bool ConfigurationReader::checkConfigurationData(ConfigurationData& data) const {
	// checking key map for duplicate entries...
	std::set<sf::Keyboard::Key> values;
	for (auto& it : data.mainKeyMap) {
		// "KeyCount" is allowed and is interpreted as "not set"
		if (it.second == sf::Keyboard::KeyCount) continue;
		// insert.second will be false if the value is already in the set
		if (!values.insert(it.second).second) {
			g_logger->logError("ConfigurationReader", "Inconsistent key map (main), a keyboard value appears twice for different keys. Key: " + std::to_string(it.second));
			return false;
		}
	}
	// values mustn't be cleared here! the alternative key map should not have values already present in the main one.
	for (auto& it : data.alternativeKeyMap) {
		// "KeyCount" is allowed and is interpreted as "key not set"
		if (it.second == sf::Keyboard::KeyCount) continue;
		// insert.second will be false if the value is already in the set
		if (!values.insert(it.second).second) {
			g_logger->logError("ConfigurationReader", "Inconsistent key map (alternative), a keyboard value appears twice for different keys. Key: " + std::to_string(it.second));
			return false;
		}
	}

	return true;
}

bool ConfigurationReader::readLanguage(const std::string& line, ConfigurationData& data) const {
	size_t colon = line.find(':');
	if (colon == std::string::npos || line.length() < colon + 1) {
		g_logger->logError("ConfigurationReader", "No colon found after language tag or no value after colon.");
		return false;
	}
	Language language = static_cast<Language>(atoi(line.substr(colon + 1).c_str()));
	if (language >= Language::MAX || language <= Language::VOID) {
		g_logger->logError("ConfigurationReader", "Language id not recognized.");
		return false;
	}
	data.language = language;
	return true;
}

bool ConfigurationReader::readDisplayMode(const std::string& line, ConfigurationData& data) const {
	size_t colon = line.find(':');
	if (colon == std::string::npos || line.length() < colon + 1) {
		g_logger->logError("ConfigurationReader", "No colon found after display mode tag or no value after colon.");
		return false;
	}
	DisplayMode mode = static_cast<DisplayMode>(atoi(line.substr(colon + 1).c_str()));
	if (mode >= DisplayMode::MAX || mode <= DisplayMode::VOID) {
		g_logger->logError("ConfigurationReader", "Display mode not recognized.");
		return false;
	}
	data.displayMode = mode;
	return true;
}

bool ConfigurationReader::readVSyncOn(const std::string& line, ConfigurationData& data) const {
	size_t colon = line.find(':');
	if (colon == std::string::npos || line.length() < colon + 1) {
		g_logger->logError("ConfigurationReader", "No colon found after VSync on tag or no value after colon.");
		return false;
	}
	bool vsyncOn = (atoi(line.substr(colon + 1).c_str()) != 0);
	data.isVSyncEnabled = vsyncOn;
	return true;
}

bool ConfigurationReader::readSoundVolumeMusic(const std::string& line, ConfigurationData& data) const {
	size_t colon = line.find(':');
	if (colon == std::string::npos || line.length() < colon + 1) {
		g_logger->logError("ConfigurationReader", "No colon found after music volume tag or no value after colon");
		return false;
	}
	int volume = atoi(line.substr(colon + 1).c_str());
	if (volume > 100 || volume < 0) {
		g_logger->logWarning("ConfigurationReader", "Music volume has an invalid value, is left unchanged.");
		return true;
	}
	data.volumeMusic = volume;
	return true;
}

bool ConfigurationReader::readSoundVolumeSound(const std::string& line, ConfigurationData& data) const {
	size_t colon = line.find(':');
	if (colon == std::string::npos || line.length() < colon + 1) {
		g_logger->logError("ConfigurationReader", "No colon found after sound volume tag or no value after colon");
		return false;
	}
	int volume = atoi(line.substr(colon + 1).c_str());
	if (volume > 100 || volume < 0) {
		g_logger->logWarning("ConfigurationReader", "Sound volume has an invalid value, is left unchanged.");
		return true;
	}
	data.volumeSound = volume;
	return true;
}

bool ConfigurationReader::readFPSMax(const std::string& line, ConfigurationData& data) const {
	size_t colon = line.find(':');
	if (colon == std::string::npos || line.length() < colon + 1) {
		g_logger->logError("ConfigurationReader", "No colon found after fps max tag or no value after colon");
		return false;
	}
	int fps = atoi(line.substr(colon + 1).c_str());
	if (fps > 100 || fps < 30) {
		g_logger->logWarning("ConfigurationReader", "Max FPS has an invalid value, is left unchanged.");
		return true;
	}
	data.maxFPS = fps;
	return true;
}

bool ConfigurationReader::readSoundOn(const std::string& line, ConfigurationData& data) const {
	return readBoolean(line, data.isSoundOn);
}

bool ConfigurationReader::readFPSLimitOn(const std::string& line, ConfigurationData& data) const {
	return readBoolean(line, data.isFPSLimited);
}

bool ConfigurationReader::readQuickcastOn(const std::string& line, ConfigurationData& data) const {
	return readBoolean(line, data.isQuickcast);
}

bool ConfigurationReader::readHintsOn(const std::string& line, ConfigurationData& data) const {
	return readBoolean(line, data.isDisplayHints);
}

bool ConfigurationReader::readQuestmarkersOn(const std::string& line, ConfigurationData& data) const {
	return readBoolean(line, data.isDisplayQuestMarkers);
}

bool ConfigurationReader::readMultithreadingOn(const std::string& line, ConfigurationData& data) const {
	return readBoolean(line, data.isMultithreading);
}

bool ConfigurationReader::readDebugRenderingOn(const std::string& line, ConfigurationData& data) const {
	return readBoolean(line, data.isDebugRendering);
}

bool ConfigurationReader::readSmoothingOn(const std::string& line, ConfigurationData& data) const {
	return readBoolean(line, data.isSmoothing);
}

bool ConfigurationReader::readAutotargetOn(const std::string& line, ConfigurationData& data) const {
	return readBoolean(line, data.isAutotarget);
}

bool ConfigurationReader::readMainInputMapping(const std::string& line, ConfigurationData& data) const {
	size_t colon = line.find(':');
	if (colon == std::string::npos || line.length() < colon + 1) {
		g_logger->logError("ConfigurationReader", "No colon found after input mapping tag or no value after colon.");
		return false;
	}
	Key key = static_cast<Key>(atoi(line.substr(colon + 1).c_str()));
	if (key >= Key::MAX || key <= Key::VOID) {
		g_logger->logError("ConfigurationReader", "Key id not recognized.");
		return false;
	}
	size_t comma = line.find(',');
	if (comma == std::string::npos || line.length() < comma + 1) {
		g_logger->logError("ConfigurationReader", "No comma found after key integer (main input mapping tag) or no value after comma.");
		return false;
	}
	sf::Keyboard::Key keyboardKey = static_cast<sf::Keyboard::Key>(atoi(line.substr(comma + 1).c_str()));
	if (keyboardKey > sf::Keyboard::Key::KeyCount || keyboardKey <= sf::Keyboard::Key::Unknown) {
		g_logger->logError("ConfigurationReader", "sf Keyboard Key id not recognized.");
		return false;
	}
	data.mainKeyMap[key] = keyboardKey;
	return true;
}

bool ConfigurationReader::readAlternativeInputMapping(const std::string& line, ConfigurationData& data) const {
	size_t colon = line.find(':');
	if (colon == std::string::npos || line.length() < colon + 1) {
		g_logger->logError("ConfigurationReader", "No colon found after input mapping tag or no value after colon.");
		return false;
	}
	Key key = static_cast<Key>(atoi(line.substr(colon + 1).c_str()));
	if (key >= Key::MAX || key <= Key::VOID) {
		g_logger->logError("ConfigurationReader", "Key id not recognized.");
		return false;
	}
	size_t comma = line.find(',');
	if (comma == std::string::npos || line.length() < comma + 1) {
		g_logger->logError("ConfigurationReader", "No comma found after key integer (alternative input mapping tag) or no value after comma.");
		return false;
	}
	sf::Keyboard::Key keyboardKey = static_cast<sf::Keyboard::Key>(atoi(line.substr(comma + 1).c_str()));
	if (keyboardKey > sf::Keyboard::Key::KeyCount || keyboardKey <= sf::Keyboard::Key::Unknown) {
		g_logger->logError("ConfigurationReader", "sf Keyboard Key id not recognized.");
		return false;
	}
	data.alternativeKeyMap[key] = keyboardKey;
	return true;
}

bool ConfigurationReader::readBoolean(const std::string& line, bool& data) const {
	size_t colon = line.find(':');
	if (colon == std::string::npos || line.length() < colon + 1) {
		g_logger->logError("ConfigurationReader", "No colon found after smoothing on tag or no value after colon.");
		return false;
	}
	bool value = (atoi(line.substr(colon + 1).c_str()) != 0);
	data = value;
	return true;
}


