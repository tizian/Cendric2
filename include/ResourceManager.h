#pragma once

#include <map>

#include "global.h"
#include "Logger.h"
#include "Enums/ResourceID.h"
#include "Enums/ErrorID.h"

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	// initializes the filename map
	void init();

	void deleteResource(ResourceID id);
	// deletes all spell + dynamic tile + enemy resources
	void deleteLevelResources();
	// loads all spell + dynamic tile + enemy resources
	// called before loading a level so that the game won't lag when using a new spell.
	void loadLevelResources();
	void deleteResource(std::string filename);

	void setError(ErrorID id, std::string& description);

	sf::Texture* getTexture(ResourceID id);
	sf::Texture* getTexture(const std::string& filename);
	sf::Font* getFont(ResourceID id);
	sf::Font* getFont(std::string& filename);
	char* getFilename(ResourceID id);
	const std::pair<ErrorID, std::string>* pollError() const;

private:
	std::map<std::string, sf::Texture> m_textures;
	std::map<std::string, sf::Font> m_fonts;
	// a map that is filled with all ResourceTags and the corresponding filenames
	std::map <ResourceID, std::string> m_fileNames;
	// this pair stores resource errors and gets checked in every game loop iteration. mostly and hopefully void.
	std::pair<ErrorID, std::string> m_currentError;
};