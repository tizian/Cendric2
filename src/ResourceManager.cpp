#include "stdafx.h"

ResourceManager* g_resourceManager;

ResourceManager::ResourceManager()
{

}
ResourceManager::~ResourceManager()
{

}

void ResourceManager::load()
{
	// loads main character at the moment
	if (!m_textures.count("main_character"))
	{
		Texture* mainCharacter = new Texture();
		if (!mainCharacter->loadFromFile("res/assets/cendric/cendric_spritesheet_walking.png"))
		{
			printf("ResourceManager: Error at opening file res/assets/cendric/cendric_spritesheet_walking.png \n");
			return;
		}
		m_textures.insert({ "main_character", mainCharacter });
	}
}

Texture& ResourceManager::getTexture(const std::string& mId)
{
	return *m_textures[mId];
}

