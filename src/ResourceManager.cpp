#include "ResourceManager.h"

using namespace std;

ResourceManager* g_resourceManager;

ResourceManager::ResourceManager() : m_textures(), m_fileNames(), m_currentError(ErrorID::Void, "")
{
}

ResourceManager::~ResourceManager()
{
	m_textures.clear();
	m_fileNames.clear();
	m_fonts.clear();
}

void ResourceManager::init()
{
	m_fileNames.insert(
	{
		{ ResourceID::Map_testmap, "res/map/testmap/testmap.dricmap" },
		{ ResourceID::Level_testlevel, "res/level/testlevel/testlevel.dric" },
		{ ResourceID::Font_copperplateGothicBold, "res/fonts/copperplate_gothic_bold.ttf" },
		{ ResourceID::Texture_mainChar, "res/assets/cendric/spritesheet_cendric_level.png" },
		{ ResourceID::Texture_mapMainChar, "res/assets/cendric/spritesheet_cendric_map.png" },
		{ ResourceID::Texture_spell_fire, "res/assets/spells/spritesheet_spell_fire.png" },
		{ ResourceID::Texture_spell_forcefield, "res/assets/spells/spritesheet_spell_forcefield.png" },
		{ ResourceID::Texture_spell_ice, "res/assets/spells/spritesheet_spell_ice.png" },
		{ ResourceID::Texture_spell_chop, "res/assets/spells/spritesheet_spell_chop.png" },
		{ ResourceID::Texture_tile_water, "res/assets/dynamic_tiles/spritesheet_tiles_water.png" },
		{ ResourceID::Texture_tile_ice, "res/assets/dynamic_tiles/spritesheet_tiles_ice.png" },
		{ ResourceID::Texture_tile_crumblyblock, "res/assets/dynamic_tiles/spritesheet_tiles_crumblyblock.png" },
		{ ResourceID::Texture_screen_splash, "res/screens/screen_splash.png" },
		{ ResourceID::Texture_screen_loading, "res/screens/screen_loading.png" },
		{ ResourceID::Texture_screen_error_fileNotFound, "res/screens/screen_error_filenotfound.png" },
		{ ResourceID::Texture_screen_error_dataCorrupted, "res/screens/screen_error_datacorrupted.png" },
		{ ResourceID::Texture_screen_menu, "res/screens/screen_menu.png" },
		{ ResourceID::Texture_weapon_icestaff, "res/assets/weapons/spritesheet_staff_ice.png" }
	});
}

sf::Texture* ResourceManager::getTexture(std::string& filename)
{
	// does the texture exist yet?
	for (std::map<std::string, sf::Texture>::iterator it = m_textures.begin();
		it != m_textures.end();
		++it)
	{
		if (filename.compare(it->first) == 0)
		{
			return &(it->second);
		}
	}

	// the texture doesn't exist. Create and save it.
	sf::Texture texture;

	// search project's main directory
	if (texture.loadFromFile(filename))
	{
		m_textures[filename] = texture;
		g_logger->logInfo("ResourceManager", std::string(filename) + ": loading texture");
		return &m_textures[filename];
	}

	g_logger->logError("ResourceManager", "Texture could not be loaded from file: " + std::string(filename));
	setError(ErrorID::Error_fileNotFound, "Texture could not be loaded from file: " + filename);
	m_textures[filename] = texture;
	return &m_textures[filename];
}

sf::Texture* ResourceManager::getTexture(ResourceID id)
{
	return getTexture(m_fileNames[id]);
}

sf::Font* ResourceManager::getFont(std::string& filename)
{
	// does the font exist yet?
	for (std::map<std::string, sf::Font>::iterator it = m_fonts.begin();
		it != m_fonts.end();
		++it)
	{
		if (filename.compare(it->first) == 0)
		{
			return &(it->second);
		}
	}

	// the font doesn't exist. Create and save it.
	sf::Font font;

	// search project's main directory
	if (font.loadFromFile(filename))
	{
		m_fonts[filename] = font;
		g_logger->logInfo("ResourceManager", std::string(filename) + ": loading font");
		return &m_fonts[filename];
	}

	g_logger->logError("ResourceManager", "Font could not be loaded from file: " + std::string(filename));
	setError(ErrorID::Error_fileNotFound, "Font could not be loaded from file: " + filename);
	m_fonts[filename] = font;
	return &m_fonts[filename];
}

sf::Font* ResourceManager::getFont(ResourceID id)
{
	return getFont(m_fileNames[id]);
}

void ResourceManager::deleteResource(ResourceID id)
{
	deleteResource(m_fileNames[id]);
}

void ResourceManager::deleteResource(std::string filename)
{
	// delete texture
	std::map<std::string, sf::Texture>::iterator textureIt = m_textures.find(filename);
	if (textureIt != m_textures.end())
	{
		m_textures.erase(textureIt);
		g_logger->logInfo("ResourceManager", std::string(filename) + ": releasing texture");
		return;
	}
		
	// delete font
	std::map<std::string, sf::Font>::iterator fontIt = m_fonts.find(filename);
	if (fontIt != m_fonts.end())
	{
		m_fonts.erase(fontIt);
		g_logger->logInfo("ResourceManager", std::string(filename) + ": releasing font");
		return;
	}

	// delete sound etc...
}

char* ResourceManager::getFilename(ResourceID id)
{
	return &m_fileNames[id][0u];
}

std::pair<ErrorID, std::string>* ResourceManager::pollError()
{
	return &m_currentError;
}

void ResourceManager::setError(ErrorID id, string& description)
{
	m_currentError.first = id;
	m_currentError.second = description;
}

void ResourceManager::deleteLevelResources()
{
	// delete spell resources
	deleteResource(ResourceID::Texture_spell_chop);
	deleteResource(ResourceID::Texture_spell_fire);
	deleteResource(ResourceID::Texture_spell_ice);
	deleteResource(ResourceID::Texture_spell_forcefield);

	// delete dynamic tile resources
	deleteResource(ResourceID::Texture_tile_water);
	deleteResource(ResourceID::Texture_tile_ice);
	deleteResource(ResourceID::Texture_tile_crumblyblock);

	// delete enemy resources

	// delete item in level resources?
}


