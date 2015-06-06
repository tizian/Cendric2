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
	m_levelFileNames.clear();
	m_mapFileNames.clear();
	m_fonts.clear();
	m_bitmapFonts.clear();
}

void ResourceManager::init()
{
	m_levelFileNames.insert(
		{ LevelID::Testlevel, "res/level/testlevel/testlevel.dric" }
	);

	m_mapFileNames.insert(
		{ MapID::Testmap, "res/map/testmap/testmap.dricmap" }
	);

	m_fileNames.insert(
	{
		{ ResourceID::BitmapFont_default, "res/fonts/default_bitmap_font.png" },
		{ ResourceID::Translations, "res/translations.csv" },
		{ ResourceID::Texture_mainChar, "res/assets/cendric/spritesheet_cendric_level.png" },
		{ ResourceID::Texture_mapMainChar, "res/assets/cendric/spritesheet_cendric_map.png" },
		{ ResourceID::Texture_spell_fire, "res/assets/spells/spritesheet_spell_fire.png" },
		{ ResourceID::Texture_spell_forcefield, "res/assets/spells/spritesheet_spell_forcefield.png" },
		{ ResourceID::Texture_spell_ice, "res/assets/spells/spritesheet_spell_ice.png" },
		{ ResourceID::Texture_enemy_rat, "res/assets/enemies/spritesheet_enemy_rat.png" },
		{ ResourceID::Texture_enemy_firerat, "res/assets/enemies/spritesheet_enemy_firerat.png" },
		{ ResourceID::Texture_tile_water, "res/assets/dynamic_tiles/spritesheet_tiles_water.png" },
		{ ResourceID::Texture_tile_ice, "res/assets/dynamic_tiles/spritesheet_tiles_ice.png" },
		{ ResourceID::Texture_tile_crumblyblock, "res/assets/dynamic_tiles/spritesheet_tiles_crumblyblock.png" },
		{ ResourceID::Texture_screen_splash, "res/screens/screen_splash.png" },
		{ ResourceID::Texture_screen_splash_fireanimation, "res/assets/misc/spritesheet_fireanimation.png" },
		{ ResourceID::Texture_screen_loading, "res/screens/screen_loading.png" },
		{ ResourceID::Texture_screen_error_fileNotFound, "res/screens/screen_error_filenotfound.png" },
		{ ResourceID::Texture_screen_error_dataCorrupted, "res/screens/screen_error_datacorrupted.png" },
		{ ResourceID::Texture_screen_menu, "res/screens/screen_menu.png" },
		{ ResourceID::Texture_screen_credits, "res/screens/screen_credits.png" },
		{ ResourceID::Texture_levelitems, "res/assets/items/spritesheet_levelitems.png" },
		{ ResourceID::Texture_items, "res/assets/items/spritesheet_items.png" },
		{ ResourceID::Texture_head_wizardhat_grey, "res/assets/equipment/head/spritesheet_head_wizardhat_grey.png" },
		{ ResourceID::Texture_head_wizardhat_blue, "res/assets/equipment/head/spritesheet_head_wizardhat_blue.png" },
		{ ResourceID::Texture_weapon_icestaff, "res/assets/equipment/weapon/spritesheet_staff_ice.png" },
		{ ResourceID::Texture_weapon_rustysword, "res/assets/equipment/weapon/spritesheet_weapon_rustysword.png" }
	});

	// font should be always loaded to avoid lags when loading later
	getBitmapFont(ResourceID::BitmapFont_default);	// TODO: Comment above maybe doesn't apply to bitmap fonts...?
}

sf::Texture* ResourceManager::getTexture(const std::string& filename)
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
    std::string tmp = "Texture could not be loaded from file: " + filename;
	setError(ErrorID::Error_fileNotFound, tmp);
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
    std::string tmp = "Font could not be loaded from file: " + filename;
	setError(ErrorID::Error_fileNotFound, tmp);
	m_fonts[filename] = font;
	return &m_fonts[filename];
}

sf::Font* ResourceManager::getFont(ResourceID id)
{
	return getFont(m_fileNames[id]);
}

BitmapFont* ResourceManager::getBitmapFont(std::string& filename)
{
	// does the font exist yet?
	for (auto it = m_bitmapFonts.begin();
		it != m_bitmapFonts.end();
		++it)
	{
		if (filename.compare(it->first) == 0)
		{
			return &(it->second);
		}
	}

	// the font doesn't exist. Create and save it.
	BitmapFont font;

	// search project's main directory
	if (font.loadFromFile(filename))
	{
		m_bitmapFonts[filename] = font;
		g_logger->logInfo("ResourceManager", std::string(filename) + ": loading bitmap font");
		return &m_bitmapFonts[filename];
	}

	g_logger->logError("ResourceManager", "Bitmap Font could not be loaded from file: " + std::string(filename));
	std::string tmp = "Bitmap Font could not be loaded from file: " + filename;
	setError(ErrorID::Error_fileNotFound, tmp);
	m_bitmapFonts[filename] = font;
	return &m_bitmapFonts[filename];
}

BitmapFont* ResourceManager::getBitmapFont(ResourceID id)
{
	return getBitmapFont(m_fileNames[id]);
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

	// delete bitmap font
	auto bitmapFontIt = m_bitmapFonts.find(filename);
	if (bitmapFontIt != m_bitmapFonts.end())
	{
		m_bitmapFonts.erase(bitmapFontIt);
		g_logger->logInfo("ResourceManager", std::string(filename) + ": releasing bitmap font");
		return;
	}

	// delete sound etc...
}

char* ResourceManager::getFilename(ResourceID id)
{
	return &m_fileNames[id][0u];
}

char* ResourceManager::getFilename(MapID id)
{
	return &m_mapFileNames[id][0u];
}

char* ResourceManager::getFilename(LevelID id)
{
	return &m_levelFileNames[id][0u];
}

const std::pair<ErrorID, std::string>* ResourceManager::pollError() const
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
	deleteResource(ResourceID::Texture_spell_fire);
	deleteResource(ResourceID::Texture_spell_ice);
	deleteResource(ResourceID::Texture_spell_forcefield);

	// delete dynamic tile resources
	deleteResource(ResourceID::Texture_tile_water);
	deleteResource(ResourceID::Texture_tile_ice);
	deleteResource(ResourceID::Texture_tile_crumblyblock);

	// delete enemy resources
	deleteResource(ResourceID::Texture_enemy_rat);
	deleteResource(ResourceID::Texture_enemy_firerat);

	// delete item in level resources
	deleteResource(ResourceID::Texture_levelitems);
}

void ResourceManager::loadLevelResources()
{
	// load spell resources
	getTexture(ResourceID::Texture_spell_fire);
	getTexture(ResourceID::Texture_spell_ice);
	getTexture(ResourceID::Texture_spell_forcefield);

	// load dynamic tile resources
	getTexture(ResourceID::Texture_tile_water);
	getTexture(ResourceID::Texture_tile_ice);
	getTexture(ResourceID::Texture_tile_crumblyblock);
}


