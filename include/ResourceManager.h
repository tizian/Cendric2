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

	const Texture& getTexture(ResourceID id);
	char* getFilename(ResourceID id);

	void deleteResource(ResourceID id);

private:
	map<ResourceID, sf::Texture> m_textures;

	// \brief a map that is filled with all ResourceTags and the corresponding filenames
	map <ResourceID, std::string> m_fileNames;
};