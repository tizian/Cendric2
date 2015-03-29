#include "stdafx.h"

enum class ResourceID
{
	Texture_mainChar,
	Level_testLevel,
	Texture_splashScreen,
	Texture_menuScreen
};

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	// \brief initializes the filename map
	void init();

	Texture* getTexture(ResourceID id);
	Texture* getTexture(std::string& filename);
	char* getFilename(ResourceID id);

	void deleteResource(ResourceID id);
	void deleteResource(std::string filename);

private:
	map<std::string, sf::Texture> m_textures;

	// \brief a map that is filled with all ResourceTags and the corresponding filenames
	map <ResourceID, std::string> m_fileNames;
};