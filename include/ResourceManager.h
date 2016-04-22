#pragma once

#include <map>

#include "global.h"
#include "Logger.h"

#include "Enums/ResourceID.h"
#include "Enums/ErrorID.h"

#include "Structs/ConfigurationData.h"
#include "FileIO/ConfigurationReader.h"

#include "GUI/BitmapFont.h"

class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	// initializes its maps & data
	void init();

	void deleteResource(ResourceID id);
	void deleteResource(const std::string& filename);

	void deleteLevelResources();
	void loadLevelResources();

	void deleteMapResources();
	void loadMapResources();

	void setError(ErrorID id, const std::string& description);

	// loads a sound and applies the current configuration to it (sound on/off, volume) and starts it.
	// if the sound is already playing, it won't start again, unless the argument "force" is set to true
	void playSound(sf::Sound& sound, ResourceID id, bool force = false);
	// streams a music and applies the current configuration to it (sound on/off, volume), starts and loops it.
	// if the music is already playing, it won't do anything.
	// if another music is playing, it will stop that and start the new one.
	// the playing offset is an optional parameter.
	void playMusic(const std::string& filename, const sf::Time& playingOffset = sf::Time::Zero);
	// stops the background music
	void stopMusic();

	sf::Texture* getTexture(ResourceID id);
	sf::Texture* getTexture(const std::string& filename);
	sf::SoundBuffer* getSoundBuffer(ResourceID id);
	sf::SoundBuffer* getSoundBuffer(const std::string& filename);
	sf::Font* getFont(ResourceID id);
	sf::Font* getFont(const std::string& filename);
	BitmapFont* getBitmapFont(ResourceID id);
	BitmapFont* getBitmapFont(const std::string& filename);

	std::string getFilename(ResourceID id);
	const std::pair<ErrorID, std::string>* pollError() const;
	ConfigurationData& getConfiguration();

private:
	std::map<std::string, sf::Texture*> m_textures;
	std::map<std::string, sf::SoundBuffer*> m_soundBuffers;
	std::map<std::string, BitmapFont*> m_bitmapFonts;
	std::map<std::string, sf::Font*> m_fonts;
	// the current background music and its path
	std::pair<std::string, sf::Music> m_currentMusic;
	// a map that is filled with all ResourceTags and the corresponding filenames
	std::map <ResourceID, std::string> m_fileNames;
	// this pair stores resource errors and gets checked in every game loop iteration. mostly and hopefully void.
	std::pair<ErrorID, std::string> m_currentError;
	// configuration can be manipulated by the user
	ConfigurationData m_configuration;
};