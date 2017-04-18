#pragma once

#include "global.h"
#include "Logger.h"

#include "Enums/ResourceType.h"
#include "Enums/ErrorID.h"

#include "Structs/ConfigurationData.h"
#include "FileIO/ConfigurationReader.h"

#include "GUI/BitmapFont.h"

class Item;

struct BackgroundMusic {
	BackgroundMusic() {
		currentMusic = new sf::Music();
		previousMusic = new sf::Music();
		fadingTime = sf::Time::Zero;
		isFading = false;
	}

	~BackgroundMusic() {
		delete currentMusic;
		delete previousMusic;
	}

	std::string path;
	sf::Music* currentMusic;
	sf::Music* previousMusic;
	sf::Time fadingTime;
	bool isFading;
	
	const sf::Time FADING_TIME = sf::seconds(1.5f);
};

class ResourceManager final {
public:
	ResourceManager();
	~ResourceManager();

	// deletes unique resources for a specific owner
	void deleteUniqueResources(void* owner);

	void deleteLevelResources();
	void loadLevelResources();

	void deleteMapResources();
	void loadMapResources();

	void deleteItemResources();

	void setError(ErrorID id, const std::string& description);

	// plays a sound that is already loaded and applies the current configuration to it (sound on/off, volume) and starts it.
	// the scale is the volume scale, has to range from 0 to 1.f.
	void playSound(const std::string& filename, bool loop = false, float scale = 1.f);
	void playSound(sf::Sound& sound, const std::string& filename, bool force, bool loop = false, float scale = 1.f);
	// streams a music and applies the current configuration to it (sound on/off, volume), starts and loops it.
	// if the music is already playing, it won't do anything.
	// if another music is playing, it will stop that fade in the new one.
	// the playing offset and looping are optional parameters.
	void playMusic(const std::string& filename, bool looping = true);
	void updateMusic(const sf::Time& frameTime);
	void notifyVolumeChanged();

	// loads a texture found at filename. If the resource type is Unique, the owner must be specified.
	void loadTexture(const std::string& filename, ResourceType type, void* owner = nullptr);
	// loads a soundbuffer found at filename. If the resource type is Unique, the owner must be specified.
	void loadSoundbuffer(const std::string& filename, ResourceType type, void* owner = nullptr);
	// loads a font found at filename. If the resource type is Unique, the owner must be specified.
	void loadFont(const std::string& filename, ResourceType type, void* owner = nullptr);
	// loads a bitmap font found at filename. If the resource type is Unique, the owner must be specified.
	void loadBitmapFont(const std::string& filename, ResourceType type, void* owner = nullptr);
	
	sf::Texture* getTexture(const std::string& filename);
	sf::SoundBuffer* getSoundBuffer(const std::string& filename);
	sf::Font* getFont(const std::string& filename);
	BitmapFont* getBitmapFont(const std::string& filename);
	// be aware that this will return nullptr in case of an invalid item.
	Item* getItem(const std::string& itemID);

	const std::pair<ErrorID, std::string>* pollError() const;
	ConfigurationData& getConfiguration();

private:
	void init();
	void deleteResource(const std::string& filename);
	// convenience template function, like baws.
	template<typename T> void loadResource(std::map<std::string, T*>& holder, const std::string& typeName, const std::string& filename, ResourceType type, void* owner = nullptr);

	// this vector holds the level resources that are currently loaded
	std::vector<std::string> m_levelResources;
	// this vector holds the map resources that are currently loaded
	std::vector<std::string> m_mapResources;
	// this map holds the resource keys (filenames) in the four maps below and their corresponding owners
	std::map<void*, std::vector<std::string>> m_resourceOwners;
	std::map<std::string, sf::Texture*> m_textures;
	std::map<std::string, sf::SoundBuffer*> m_soundBuffers;
	std::map<std::string, BitmapFont*> m_bitmapFonts;
	std::map<std::string, sf::Font*> m_fonts;
	std::map<std::string, Item*> m_items;

	// a pool with sound buffers
	std::vector<sf::Sound> m_soundPool;
	std::set<std::string> m_frameSounds;
	size_t m_nextSoundIndex;
	static const size_t SOUND_POOL_SIZE;
	// the current background music and its path
	BackgroundMusic m_music;
	// this pair stores resource errors and gets checked in every game loop iteration. mostly and hopefully void.
	std::pair<ErrorID, std::string> m_currentError;
	// configuration can be manipulated by the user
	ConfigurationData m_configuration;
};