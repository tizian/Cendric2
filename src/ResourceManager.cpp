#include "stdafx.h"

ResourceManager* g_resourceManager;

ResourceManager::ResourceManager() : m_textures(), m_fileNames()
{
}
ResourceManager::~ResourceManager()
{
	m_textures.clear();
	m_fileNames.clear();
}

void ResourceManager::init()
{
	m_fileNames.insert(
	{
		{ ResourceID::Level_testLevel, "res/level/testlevel.dric" },
		{ ResourceID::Texture_mainChar, "res/assets/cendric/cendric_spritesheet.png" }
	});
}

const sf::Texture& ResourceManager::getTexture(ResourceID id)
{
	// does the texture exist yet?
	for (std::map<ResourceID, sf::Texture>::const_iterator it = m_textures.begin();
		it != m_textures.end();
		++it)
	{
		if (id == it->first)
		{
			return it->second;
		}
	}

	// the texture doesn't exist. Create and save it.
	sf::Texture texture;

	// Search project's main directory
	if (texture.loadFromFile(m_fileNames[id]))
	{
		m_textures[id] = texture;
		std::cout << "ResourceManager: " << m_fileNames[id] << ": loading texture.\n";
		return m_textures[id];
	}

	std::cout << "ResourceManager: ERROR: Texture could not be loaded from file: " << m_fileNames[id] << ".\n";
	m_textures[id] = texture;
	return m_textures[id];
}

void ResourceManager::deleteResource(ResourceID id)
{
	// delete texture
	std::map<ResourceID, sf::Texture>::const_iterator it = m_textures.find(id);
	if (it != m_textures.end())
		m_textures.erase(it);

	// delete sound etc...
}

char* ResourceManager::getFilename(ResourceID id)
{
	return &m_fileNames[id][0u];
}


