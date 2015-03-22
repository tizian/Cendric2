#include "stdafx.h"

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	// \brief loads resources. TODO: should be specified with IDs to load
	void load();

	Texture& getTexture(const std::string& mId);

private:
	map<std::string, Texture*> m_textures;
};