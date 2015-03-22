#include "stdafx.h"


ResourceManager::ResourceManager()
{

}
ResourceManager::~ResourceManager()
{

}

void ResourceManager::load()
{

}

Texture& ResourceManager::getTexture(const std::string& mId)
{
	return *m_textures[mId];
}

Sound& ResourceManager::getSound(const std::string& mId)
{
	return *m_sounds[mId];
}

Music& ResourceManager::getMusic(const std::string& mId)
{
	return *m_music[mId];
}
