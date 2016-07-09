#include "FileIO/ConfigurationReader.h"
#include "GlobalResource.h"


#include <cstring>

using namespace std;

bool ConfigurationReader::readConfiguration(ConfigurationData& data) const {
	data = DEFAULT_CONFIGURATION;

	string line;
	ifstream configuration(getResourcePath(GlobalResource::CONFIGURATION_PATH));
	bool noError = true;
	if (configuration.is_open()) {
		while (getline(configuration, line)) {
			if (line.empty() || line.compare(0, 1, "#") == 0) {
				continue;
			}
			else if (line.compare(0, strlen(LANGUAGE), string(LANGUAGE)) == 0) {
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(LANGUAGE));
				noError = readLanguage(line, data);
			}
			else if (line.compare(0, strlen(MAIN_INPUT_MAPPING), string(MAIN_INPUT_MAPPING)) == 0) {
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(MAIN_INPUT_MAPPING));
				noError = readMainInputMapping(line, data);
			}
			else if (line.compare(0, strlen(ALTERNATIVE_INPUT_MAPPING), string(ALTERNATIVE_INPUT_MAPPING)) == 0) {
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(ALTERNATIVE_INPUT_MAPPING));
				noError = readAlternativeInputMapping(line, data);
			}
			else if (line.compare(0, strlen(VSYNC_ON), string(VSYNC_ON)) == 0) {
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(VSYNC_ON));
				noError = readVSyncOn(line, data);
			}
			else if (line.compare(0, strlen(SOUND_ON), string(SOUND_ON)) == 0) {
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(SOUND_ON));
				noError = readSoundOn(line, data);
			}
			else if (line.compare(0, strlen(SOUND_VOLUME_MUSIC), string(SOUND_VOLUME_MUSIC)) == 0) {
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(SOUND_VOLUME_MUSIC));
				noError = readSoundVolumeMusic(line, data);
			}
			else if (line.compare(0, strlen(SOUND_VOLUME_SOUND), string(SOUND_VOLUME_SOUND)) == 0) {
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(SOUND_VOLUME_SOUND));
				noError = readSoundVolumeSound(line, data);
			}
			else if (line.compare(0, strlen(QUICKCAST_ON), string(QUICKCAST_ON)) == 0) {
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(QUICKCAST_ON));
				noError = readQuickcastOn(line, data);
			}
			else if (line.compare(0, strlen(HINTS_ON), string(HINTS_ON)) == 0) {
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(HINTS_ON));
				noError = readHintsOn(line, data);
			}
			else if (line.compare(0, strlen(DAMAGENUMBERS_ON), string(DAMAGENUMBERS_ON)) == 0) {
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(DAMAGENUMBERS_ON));
				noError = readDamageNumbersOn(line, data);
			}
			else if (line.compare(0, strlen(DEBUGMODE_ON), string(DEBUGMODE_ON)) == 0) {
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(DEBUGMODE_ON));
				noError = readDebugModeOn(line, data);
			}
			else if (line.compare(0, strlen(DEBUGRENDERING_ON), string(DEBUGRENDERING_ON)) == 0) {
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(DEBUGRENDERING_ON));
				noError = readDebugRenderingOn(line, data);
			}
			else if (line.compare(0, strlen(SMOOTHING_ON), string(SMOOTHING_ON)) == 0) {
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(SMOOTHING_ON));
				noError = readSmoothingOn(line, data);
			}
			else if (line.compare(0, strlen(DISPLAYMODE), string(DISPLAYMODE)) == 0) {
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(DISPLAYMODE));
				noError = readDisplayMode(line, data);
			}
			else {
				g_logger->logError("ConfigurationReader", "Unknown tag found in configuration file.");
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
	if (colon == string::npos || line.length() < colon + 1) {
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
	if (colon == string::npos || line.length() < colon + 1) {
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
	if (colon == string::npos || line.length() < colon + 1) {
		g_logger->logError("ConfigurationReader", "No colon found after VSync on tag or no value after colon.");
		return false;
	}
	bool vsyncOn = (atoi(line.substr(colon + 1).c_str()) != 0);
	data.isVSyncEnabled = vsyncOn;
	return true;
}

bool ConfigurationReader::readSoundVolumeMusic(const std::string& line, ConfigurationData& data) const {
	size_t colon = line.find(':');
	if (colon == string::npos || line.length() < colon + 1) {
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
	if (colon == string::npos || line.length() < colon + 1) {
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

bool ConfigurationReader::readSoundOn(const std::string& line, ConfigurationData& data) const {
	size_t colon = line.find(':');
	if (colon == string::npos || line.length() < colon + 1) {
		g_logger->logError("ConfigurationReader", "No colon found after sound on tag or no value after colon.");
		return false;
	}
	bool soundOn = (atoi(line.substr(colon + 1).c_str()) != 0);
	data.isSoundOn = soundOn;
	return true;
}

bool ConfigurationReader::readQuickcastOn(const std::string& line, ConfigurationData& data) const {
	size_t colon = line.find(':');
	if (colon == string::npos || line.length() < colon + 1) {
		g_logger->logError("ConfigurationReader", "No colon found after quickcast on tag or no value after colon.");
		return false;
	}
	bool quickcastOn = (atoi(line.substr(colon + 1).c_str()) != 0);
	data.isQuickcast = quickcastOn;
	return true;
}

bool ConfigurationReader::readHintsOn(const std::string& line, ConfigurationData& data) const {
	size_t colon = line.find(':');
	if (colon == string::npos || line.length() < colon + 1) {
		g_logger->logError("ConfigurationReader", "No colon found after hints on tag or no value after colon.");
		return false;
	}
	bool hintsOn = (atoi(line.substr(colon + 1).c_str()) != 0);
	data.isDisplayHints = hintsOn;
	return true;
}

bool ConfigurationReader::readDamageNumbersOn(const std::string& line, ConfigurationData& data) const {
	size_t colon = line.find(':');
	if (colon == string::npos || line.length() < colon + 1) {
		g_logger->logError("ConfigurationReader", "No colon found after damage numbers on tag or no value after colon.");
		return false;
	}
	bool damageNumbersOn = (atoi(line.substr(colon + 1).c_str()) != 0);
	data.isDisplayDamageNumbers = damageNumbersOn;
	return true;
}

bool ConfigurationReader::readDebugModeOn(const std::string& line, ConfigurationData& data) const {
	size_t colon = line.find(':');
	if (colon == string::npos || line.length() < colon + 1) {
		g_logger->logError("ConfigurationReader", "No colon found after debug mode on tag or no value after colon.");
		return false;
	}
	bool debugOn = (atoi(line.substr(colon + 1).c_str()) != 0);
	data.isDebugMode = debugOn;
	return true;
}

bool ConfigurationReader::readDebugRenderingOn(const std::string& line, ConfigurationData& data) const {
	size_t colon = line.find(':');
	if (colon == string::npos || line.length() < colon + 1) {
		g_logger->logError("ConfigurationReader", "No colon found after debug rendering on tag or no value after colon.");
		return false;
	}
	bool debugOn = (atoi(line.substr(colon + 1).c_str()) != 0);
	data.isDebugRendering = debugOn;
	return true;
}

bool ConfigurationReader::readSmoothingOn(const std::string& line, ConfigurationData& data) const {
	size_t colon = line.find(':');
	if (colon == string::npos || line.length() < colon + 1) {
		g_logger->logError("ConfigurationReader", "No colon found after smoothing on tag or no value after colon.");
		return false;
	}
	bool smoothingOn = (atoi(line.substr(colon + 1).c_str()) != 0);
	data.isSmoothing = smoothingOn;
	return true;
}

bool ConfigurationReader::readMainInputMapping(const std::string& line, ConfigurationData& data) const {
	size_t colon = line.find(':');
	if (colon == string::npos || line.length() < colon + 1) {
		g_logger->logError("ConfigurationReader", "No colon found after input mapping tag or no value after colon.");
		return false;
	}
	Key key = static_cast<Key>(atoi(line.substr(colon + 1).c_str()));
	if (key >= Key::MAX || key <= Key::VOID) {
		g_logger->logError("ConfigurationReader", "Key id not recognized.");
		return false;
	}
	size_t comma = line.find(',');
	if (comma == string::npos || line.length() < comma + 1) {
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
	if (colon == string::npos || line.length() < colon + 1) {
		g_logger->logError("ConfigurationReader", "No colon found after input mapping tag or no value after colon.");
		return false;
	}
	Key key = static_cast<Key>(atoi(line.substr(colon + 1).c_str()));
	if (key >= Key::MAX || key <= Key::VOID) {
		g_logger->logError("ConfigurationReader", "Key id not recognized.");
		return false;
	}
	size_t comma = line.find(',');
	if (comma == string::npos || line.length() < comma + 1) {
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


