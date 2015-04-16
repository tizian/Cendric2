#pragma once

#include<map>

#include "global.h"

enum class ResourceID
{	
	Void,
	Map_testmap,
	Level_testlevel,
	Font_copperplateGothicBold,
	Texture_mainChar,
	Texture_mapMainChar,
	Texture_spell_chop,
	Texture_spell_fire,
	Texture_spell_ice,
	Texture_spell_forcefield,
	Texture_screen_splash,
	Texture_screen_loading,
	Texture_screen_error_fileNotFound,
	Texture_screen_error_dataCorrupted,
	Texture_screen_menu
};

enum class ErrorID
{
	Void,
	Error_fileNotFound,
	Error_dataCorrupted
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
	sf::Font* getFont(ResourceID id);
	sf::Font* getFont(std::string& filename);
	char* getFilename(ResourceID id);

	void deleteResource(ResourceID id);
	void deleteResource(std::string filename);
	std::pair<ErrorID, std::string>* pollError();
	void setError(ErrorID id, std::string& description);

private:
	std::map<std::string, sf::Texture> m_textures;
	std::map<std::string, sf::Font> m_fonts;

	// \brief a map that is filled with all ResourceTags and the corresponding filenames
	std::map <ResourceID, std::string> m_fileNames;
	
	// this pair stores resource errors and gets checked in every game loop iteration. mostly and hopefully void.
	std::pair<ErrorID, std::string> m_currentError;
};