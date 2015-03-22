#include "stdafx.h"

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	// \brief loads resources. TODO: should be specified with IDs to load
	void load();

	Texture& getTexture(const std::string& mId);
	Sound& getSound(const std::string& mId);
	Music& getMusic(const std::string& mId);

private:
	map<std::string, std::unique_ptr<Texture>> m_textures;
	map<std::string, std::unique_ptr<Sound>> m_sounds;
	map<std::string, std::unique_ptr<Music>> m_music;
};