#pragma once

#include<map>

#include "global.h"

enum class ResourceID
{	
	Void,
	Map_testmap,
	Level_testlevel,
	Texture_mainChar,
	Texture_mapMainChar,
	Texture_spell_fire,
	Texture_spell_ice,
	Texture_screen_splash,
	Texture_screen_loading,
	Texture_screen_menu
};

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	// \brief initializes the filename map
	void init();

	sf::Texture* getTexture(ResourceID id);
	sf::Texture* getTexture(std::string& filename);
	char* getFilename(ResourceID id);

	void deleteResource(ResourceID id);
	void deleteResource(std::string filename);

private:
	std::map<std::string, sf::Texture> m_textures;

	// \brief a map that is filled with all ResourceTags and the corresponding filenames
	std::map <ResourceID, std::string> m_fileNames;
};