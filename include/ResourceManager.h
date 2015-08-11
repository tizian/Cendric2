#pragma once

#include <map>

#include "global.h"
#include "Logger.h"

#include "Enums/ResourceID.h"
#include "Enums/ErrorID.h"
#include "Enums/MapID.h"
#include "Enums/LevelID.h"

#include "Structs/ConfigurationData.h"
#include "FileIO/ConfigurationReader.h"
#include "FileIO/ItemReader.h"

#include "GUI/BitmapFont.h"

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	// initializes its maps & data
	void init();

	void deleteResource(ResourceID id);
	// deletes all spell + dynamic tile + enemy resources
	void deleteLevelResources();
	// loads all spell + dynamic tile + enemy resources
	// called before loading a level so that the game won't lag when using a new spell.
	void loadLevelResources();
	void deleteResource(const std::string &filename);

	void setError(ErrorID id, const std::string &description);

	sf::Texture *getTexture(ResourceID id);
	sf::Texture *getTexture(const std::string& filename);
	sf::Font *getFont(ResourceID id);
	sf::Font *getFont(const std::string &filename);
	BitmapFont *getBitmapFont(ResourceID id);
	BitmapFont *getBitmapFont(const std::string &filename);

	const ItemBean *getItemBean(const std::string &id);
	char* getFilename(ResourceID id);
	char* getFilename(LevelID id);
	char* getFilename(MapID id);
	const std::pair<ErrorID, std::string> *pollError() const;
	ConfigurationData &getConfiguration();
	int getMaxFPS() const;

private:
	std::map<std::string, sf::Texture> m_textures;
	std::map<std::string, BitmapFont> m_bitmapFonts;
	std::map<std::string, sf::Font> m_fonts;
	// a map that is filled with all ResourceTags and the corresponding filenames
	std::map <ResourceID, std::string> m_fileNames;
	// a map that is filled with all LevelIDs and their filenames
	std::map <LevelID, std::string> m_levelFileNames;
	// a map that is filled with all MapIDs and their filenames
	std::map <MapID, std::string> m_mapFileNames;
	// this pair stores resource errors and gets checked in every game loop iteration. mostly and hopefully void.
	std::pair<ErrorID, std::string> m_currentError;
	// configuration can be manipulated by the user
	ConfigurationData m_configuration;
	// all item beans are saved here
	std::map<std::string, ItemBean> m_itemMap;
};