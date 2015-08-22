#include "FileIO/ConfigurationReader.h"
#include "ResourceManager.h"

#include <cstring>

using namespace std;

bool ConfigurationReader::readConfiguration(ConfigurationData& data) const
{
	data = DEFAULT_CONFIGURATION;

	string line;
	ifstream configuration(g_resourceManager->getFilename(ResourceID::Configuration));
	bool noError = true;
	if (configuration.is_open())
	{
		while (getline(configuration, line))
		{
			if (line.empty() || line.compare(0, 1, "#") == 0)
			{
				continue;
			}
			else if (line.compare(0, strlen(LANGUAGE), string(LANGUAGE)) == 0) 
			{
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(LANGUAGE));
				noError = readLanguage(line, data);
			}
			else if (line.compare(0, strlen(MAIN_INPUT_MAPPING), string(MAIN_INPUT_MAPPING)) == 0)
			{
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(MAIN_INPUT_MAPPING));
				noError = readMainInputMapping(line, data);
			}
			else if (line.compare(0, strlen(ALTERNATIVE_INPUT_MAPPING), string(ALTERNATIVE_INPUT_MAPPING)) == 0)
			{
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(ALTERNATIVE_INPUT_MAPPING));
				noError = readAlternativeInputMapping(line, data);
			}
			else if (line.compare(0, strlen(MAX_FPS), string(MAX_FPS)) == 0)
			{
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(MAX_FPS));
				noError = readMaxFPS(line, data);
			}
			else if (line.compare(0, strlen(SOUND_ON), string(SOUND_ON)) == 0)
			{
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(SOUND_ON));
				noError = readSoundOn(line, data);
			}
			else if (line.compare(0, strlen(SOUND_VOLUME), string(SOUND_VOLUME)) == 0)
			{
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(SOUND_VOLUME));
				noError = readSoundVolume(line, data);
			}
			else if (line.compare(0, strlen(QUICKCAST_ON), string(QUICKCAST_ON)) == 0)
			{
				g_logger->log(LogLevel::Verbose, "ConfigurationReader", "found tag " + std::string(QUICKCAST_ON));
				noError = readQuickcastOn(line, data);
			}
			else
			{
				g_logger->logError("ConfigurationReader", "Unknown tag found in configuration file.");
				noError = false;
			}
			if (!noError)
			{
				break;
			}
		}
		configuration.close();
		if (!noError || !checkConfigurationData(data))
		{
			return false;
		}
	}
	else
	{
		g_logger->logWarning("ConfigurationReader", "Unable to open configuration file. Default configuration is used.");
		return false;
	}
	
	return true;
}

bool ConfigurationReader::checkConfigurationData(ConfigurationData& data) const
{
	// checking key map for duplicate entries...
	std::set<sf::Keyboard::Key> values;
	for (auto& it : data.mainKeyMap)
	{
		// "KeyCount" is allowed and is interpreted as "not set"
		if (it.second == sf::Keyboard::KeyCount) continue;
		// insert.second will be false if the value is already in the set
		if (!values.insert(it.second).second)
		{
            g_logger->logError("ConfigurationReader", "Inconsistent key map (main), a keyboard value appears twice for different keys. Key: " + std::to_string(it.second));
			return false;
		}
	}
	// values mustn't be cleared here! the alternative key map should not have values already present in the main one.
	for (auto& it : data.alternativeKeyMap)
	{
		// "KeyCount" is allowed and is interpreted as "key not set"
		if (it.second == sf::Keyboard::KeyCount) continue;
		// insert.second will be false if the value is already in the set
		if (!values.insert(it.second).second)
		{
			g_logger->logError("ConfigurationReader", "Inconsistent key map (alternative), a keyboard value appears twice for different keys. Key: " + std::to_string(it.second));
			return false;
		}
	}

	return true;
}

bool ConfigurationReader::readLanguage(const std::string& line, ConfigurationData& data) const
{
	size_t colon = line.find(':');
	if (colon == string::npos || line.length() < colon + 1)
	{
		g_logger->logError("ConfigurationReader", "No colon found after language tag or no value after colon.");
		return false;
	}
	Language language = static_cast<Language>(atoi(line.substr(colon + 1).c_str()));
	if (language >= Language::MAX || language <= Language::Void)
	{
		g_logger->logError("ConfigurationReader", "Language id not recognized.");
		return false;
	}
	data.language = language;
	return true;
}

bool ConfigurationReader::readMaxFPS(const std::string& line, ConfigurationData& data) const
{
	size_t colon = line.find(':');
	if (colon == string::npos || line.length() < colon + 1)
	{
		g_logger->logError("ConfigurationReader", "No colon found after max fps tag or no value after colon");
		return false;
	}
	int maxFPS = atoi(line.substr(colon + 1).c_str());
	if (maxFPS > 100 || maxFPS < 30)
	{
		g_logger->logWarning("ConfigurationReader", "Max FPS has an invalid value, is left unchanged.");
		return true;
	}
	data.maxFrameRate = maxFPS;
	return true;
}

bool ConfigurationReader::readSoundVolume(const std::string& line, ConfigurationData& data) const
{
	size_t colon = line.find(':');
	if (colon == string::npos || line.length() < colon + 1)
	{
		g_logger->logError("ConfigurationReader", "No colon found after sound volume tag or no value after colon");
		return false;
	}
	int volume = atoi(line.substr(colon + 1).c_str());
	if (volume > 100 || volume < 0)
	{
		g_logger->logWarning("ConfigurationReader", "Sound volume has an invalid value, is left unchanged.");
		return true;
	}
	data.volume = volume;
	return true;
}

bool ConfigurationReader::readSoundOn(const std::string& line, ConfigurationData& data) const
{
	size_t colon = line.find(':');
	if (colon == string::npos || line.length() < colon + 1)
	{
		g_logger->logError("ConfigurationReader", "No colon found after sound on tag or no value after colon.");
		return false;
	}
	bool soundOn = (atoi(line.substr(colon + 1).c_str()) != 0);
	data.isSoundOn = soundOn;
	return true;
}

bool ConfigurationReader::readQuickcastOn(const std::string& line, ConfigurationData& data) const
{
	size_t colon = line.find(':');
	if (colon == string::npos || line.length() < colon + 1)
	{
		g_logger->logError("ConfigurationReader", "No colon found after quickcast on tag or no value after colon.");
		return false;
	}
	bool quickcastOn = (atoi(line.substr(colon + 1).c_str()) != 0);
	data.isQuickcast = quickcastOn;
	return true;
}

bool ConfigurationReader::readMainInputMapping(const std::string& line, ConfigurationData& data) const
{
	size_t colon = line.find(':');
	if (colon == string::npos || line.length() < colon + 1)
	{
		g_logger->logError("ConfigurationReader", "No colon found after input mapping tag or no value after colon.");
		return false;
	}
	Key key = static_cast<Key>(atoi(line.substr(colon + 1).c_str()));
	if (key >= Key::MAX || key <= Key::VOID)
	{
		g_logger->logError("ConfigurationReader", "Key id not recognized.");
		return false;
	}
	size_t comma = line.find(',');
	if (comma == string::npos || line.length() < comma + 1)
	{
		g_logger->logError("ConfigurationReader", "No comma found after key integer (main input mapping tag) or no value after comma.");
		return false;
	}
	sf::Keyboard::Key keyboardKey = static_cast<sf::Keyboard::Key>(atoi(line.substr(comma + 1).c_str()));
	if (keyboardKey > sf::Keyboard::Key::KeyCount || keyboardKey <= sf::Keyboard::Key::Unknown)
	{
		g_logger->logError("ConfigurationReader", "sf Keyboard Key id not recognized.");
		return false;
	}
	data.mainKeyMap.insert({key, keyboardKey});
	return true;
}

bool ConfigurationReader::readAlternativeInputMapping(const std::string& line, ConfigurationData& data) const
{
	size_t colon = line.find(':');
	if (colon == string::npos || line.length() < colon + 1)
	{
		g_logger->logError("ConfigurationReader", "No colon found after input mapping tag or no value after colon.");
		return false;
	}
	Key key = static_cast<Key>(atoi(line.substr(colon + 1).c_str()));
	if (key >= Key::MAX || key <= Key::VOID)
	{
		g_logger->logError("ConfigurationReader", "Key id not recognized.");
		return false;
	}
	size_t comma = line.find(',');
	if (comma == string::npos || line.length() < comma + 1)
	{
		g_logger->logError("ConfigurationReader", "No comma found after key integer (alternative input mapping tag) or no value after comma.");
		return false;
	}
	sf::Keyboard::Key keyboardKey = static_cast<sf::Keyboard::Key>(atoi(line.substr(comma + 1).c_str()));
	if (keyboardKey > sf::Keyboard::Key::KeyCount || keyboardKey <= sf::Keyboard::Key::Unknown)
	{
		g_logger->logError("ConfigurationReader", "sf Keyboard Key id not recognized.");
		return false;
	}
	data.alternativeKeyMap.insert({ key, keyboardKey });
	return true;
}


