#include "ResourceManager.h"
#include "AnimatedSprite.h"

using namespace std;

ResourceManager *g_resourceManager;

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
	m_itemMap.clear();
}

void ResourceManager::init()
{
	m_levelFileNames.insert(
	{
		{ LevelID::RatCave, "res/level/ratcave/ratcave.dric" },
		{ LevelID::Testlevel, "res/level/testlevel/testlevel.dric" }
	});
	
	m_mapFileNames.insert(
		{ MapID::Firstmap, "res/map/firstmap/firstmap.dricmap" }
	);

	m_fileNames.insert(
	{
		{ ResourceID::BitmapFont_default, "res/fonts/default_bitmap_font.png" },
		{ ResourceID::Translations, "res/translations.csv" },
		{ ResourceID::Configuration, "config.ini" },
		{ ResourceID::Save_folder, "saves/" },
		{ ResourceID::Quicksave, "saves/quicksave.sav" },
		{ ResourceID::Items, "res/items.csv" },
		{ ResourceID::Texture_spellicons, "res/assets/spells/spritesheet_spellicons.png" },
		{ ResourceID::Texture_mainChar, "res/assets/cendric/spritesheet_cendric_level.png" },
		{ ResourceID::Texture_mapMainChar, "res/assets/cendric/spritesheet_cendric_map.png" },
		{ ResourceID::Texture_spell_fireball, "res/assets/spells/spritesheet_spell_fireball.png" },
		{ ResourceID::Texture_spell_divineshield, "res/assets/spells/spritesheet_spell_divineshield.png" },
		{ ResourceID::Texture_spell_iceball, "res/assets/spells/spritesheet_spell_iceball.png" },
		{ ResourceID::Texture_spell_aureola, "res/assets/spells/spritesheet_spell_aureola.png" },
		{ ResourceID::Texture_spell_fear, "res/assets/spells/spritesheet_spell_fear.png" },
		{ ResourceID::Texture_spell_icyambush, "res/assets/spells/spritesheet_spell_icyambush.png" },
		{ ResourceID::Texture_enemy_rat, "res/assets/enemies/spritesheet_enemy_rat.png" },
		{ ResourceID::Texture_enemy_firerat, "res/assets/enemies/spritesheet_enemy_firerat.png" },
		{ ResourceID::Texture_tile_ice, "res/assets/dynamic_tiles/spritesheet_tiles_ice.png" },
		{ ResourceID::Texture_tile_crumblyblock, "res/assets/dynamic_tiles/spritesheet_tiles_crumblyblock.png" },
		{ ResourceID::Texture_tile_torch, "res/assets/dynamic_tiles/spritesheet_tiles_torch.png" },
		{ ResourceID::Texture_tile_chest, "res/assets/dynamic_tiles/spritesheet_tiles_chest.png" },
		{ ResourceID::Texture_tile_spikesbottom, "res/assets/dynamic_tiles/spritesheet_tiles_spikesbottom.png" },
		{ ResourceID::Texture_tile_spikestop, "res/assets/dynamic_tiles/spritesheet_tiles_spikestop.png" },
		{ ResourceID::Texture_tile_frozenwater, "res/assets/dynamic_tiles/spritesheet_tiles_frozenwater.png" },
		{ ResourceID::Texture_tile_shiftableblock, "res/assets/dynamic_tiles/spritesheet_tiles_shiftableblock.png" },
		{ ResourceID::Texture_screen_splash, "res/screens/screen_splash.png" },
		{ ResourceID::Texture_screen_splash_fireanimation, "res/assets/misc/spritesheet_fireanimation.png" },
		{ ResourceID::Texture_screen_loading, "res/screens/screen_loading.png" },
		{ ResourceID::Texture_screen_error_fileNotFound, "res/screens/screen_error_filenotfound.png" },
		{ ResourceID::Texture_screen_error_dataCorrupted, "res/screens/screen_error_datacorrupted.png" },
		{ ResourceID::Texture_screen_gameover, "res/screens/screen_gameover.png" },
		{ ResourceID::Texture_screen_overlay, "res/screens/screen_overlay.png" },
		{ ResourceID::Texture_screen_menu, "res/screens/screen_menu.png" },
		{ ResourceID::Texture_debuff_fear, "res/assets/debuffs/spritesheet_debuff_fear.png" },
		{ ResourceID::Texture_debuff_stun, "res/assets/debuffs/spritesheet_debuff_stun.png" },
		{ ResourceID::Texture_screen_credits, "res/screens/screen_credits.png" },
		{ ResourceID::Texture_levelitems, "res/assets/items/spritesheet_levelitems.png" },
		{ ResourceID::Texture_items, "res/assets/items/spritesheet_items.png" },
		{ ResourceID::Texture_npcs, "res/assets/npcs/spritesheet_npcs.png" },
		{ ResourceID::Texture_dialogue, "res/assets/dialogue/spritesheet_dialogue.png" },
		{ ResourceID::Texture_inventorytabs, "res/assets/misc/spritesheet_inventorytabs.png" },
		{ ResourceID::Texture_equipmentplaceholders, "res/assets/misc/spritesheet_equipmentplaceholders.png" },
		{ ResourceID::Texture_GUI_rounded_rectangle, "res/assets/gui/rounded_rectangle.png" },
		{ ResourceID::Texture_GUI_ornament_none, "res/assets/gui/ornament_none.png" },
		{ ResourceID::Texture_GUI_ornament_small, "res/assets/gui/ornament_small.png" },
		{ ResourceID::Texture_GUI_ornament_medium, "res/assets/gui/ornament_medium.png" },
		{ ResourceID::Texture_GUI_ornament_large, "res/assets/gui/ornament_large.png" },
		{ ResourceID::Texture_GUI_arrow, "res/assets/gui/arrow.png" },
		{ ResourceID::Texture_GUI_spell_color_elemental, "res/assets/gui/spell_color_elemental.png" },
		{ ResourceID::Texture_GUI_spell_color_twilight, "res/assets/gui/spell_color_twilight.png" },
		{ ResourceID::Texture_GUI_spell_color_necromancy, "res/assets/gui/spell_color_necromancy.png" },
		{ ResourceID::Texture_GUI_spell_color_divine, "res/assets/gui/spell_color_divine.png" },
		{ ResourceID::Texture_GUI_spell_color_illusion, "res/assets/gui/spell_color_illusion.png" },
		{ ResourceID::Texture_Particle_circle, "res/assets/particles/circle.png" },
		{ ResourceID::Texture_Particle_blob, "res/assets/particles/blob.png" },
		{ ResourceID::Texture_Particle_snowflake, "res/assets/particles/snowflake.png" }
	});

	// font should be always loaded to avoid lags when loading later
	getBitmapFont(ResourceID::BitmapFont_default);	

	ConfigurationReader reader;
	if (!reader.readConfiguration(m_configuration))
	{
		m_configuration = DEFAULT_CONFIGURATION;
	}

	ItemReader itemReader;
	if (!itemReader.readItems(m_itemMap))
	{
		m_itemMap.clear();
		g_logger->logError("ResourceManager", "Items could not be loaded from file: " + std::string(getFilename(ResourceID::Items)));
		setError(ErrorID::Error_dataCorrupted, "Items could not be loaded from file: " + std::string(getFilename(ResourceID::Items)));
	}
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

sf::Font* ResourceManager::getFont(const std::string &filename)
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

BitmapFont* ResourceManager::getBitmapFont(const std::string &filename)
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

const ItemBean* ResourceManager::getItemBean(const std::string& id)
{
	if (m_itemMap.find(id) != m_itemMap.end())
	{
		return &m_itemMap.at(id);
	}
	else
	{
		g_logger->logError("ResourceManager", "Item not found with ID: " + id);
		return nullptr;
	}
}

void ResourceManager::deleteResource(ResourceID id)
{
	deleteResource(m_fileNames[id]);
}

void ResourceManager::deleteResource(const std::string &filename)
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

ConfigurationData& ResourceManager::getConfiguration()
{
	return m_configuration;
}

int ResourceManager::getMaxFPS() const
{
	return m_configuration.maxFrameRate;
}

void ResourceManager::setError(ErrorID id, const string &description)
{
	m_currentError.first = id;
	m_currentError.second = description;
}

void ResourceManager::deleteLevelResources()
{
	// delete spell resources
	deleteResource(ResourceID::Texture_spell_fireball);
	deleteResource(ResourceID::Texture_spell_iceball);
	deleteResource(ResourceID::Texture_spell_divineshield);
	deleteResource(ResourceID::Texture_spell_aureola);
	deleteResource(ResourceID::Texture_spell_fear);
	deleteResource(ResourceID::Texture_spell_icyambush);

	// delete dynamic tile resources
	deleteResource(ResourceID::Texture_tile_frozenwater);
	deleteResource(ResourceID::Texture_tile_ice);
	deleteResource(ResourceID::Texture_tile_crumblyblock);
	deleteResource(ResourceID::Texture_tile_torch);
	deleteResource(ResourceID::Texture_tile_chest);
	deleteResource(ResourceID::Texture_tile_spikesbottom);
	deleteResource(ResourceID::Texture_tile_spikestop);

	// delete enemy resources
	deleteResource(ResourceID::Texture_enemy_rat);
	deleteResource(ResourceID::Texture_enemy_firerat);

	// delete debuff resources
	deleteResource(ResourceID::Texture_debuff_fear);
	deleteResource(ResourceID::Texture_debuff_stun);

	// delete item in level resources
	deleteResource(ResourceID::Texture_levelitems);
	
	// delete game over sprite
	deleteResource(ResourceID::Texture_screen_gameover);
}

void ResourceManager::loadLevelResources()
{
	// load spell resources
	getTexture(ResourceID::Texture_spell_fireball);
	getTexture(ResourceID::Texture_spell_iceball);
	getTexture(ResourceID::Texture_spell_divineshield);
	getTexture(ResourceID::Texture_spell_aureola);
	getTexture(ResourceID::Texture_spell_fear);
	getTexture(ResourceID::Texture_spell_icyambush);

	// load dynamic tile resources
	getTexture(ResourceID::Texture_tile_frozenwater);
	getTexture(ResourceID::Texture_tile_ice);
	getTexture(ResourceID::Texture_tile_crumblyblock);
	getTexture(ResourceID::Texture_tile_torch);
	getTexture(ResourceID::Texture_tile_chest);
	getTexture(ResourceID::Texture_tile_spikestop);
	getTexture(ResourceID::Texture_tile_spikesbottom);

	// load game over sprite 
	getTexture(ResourceID::Texture_screen_gameover);

	// load debuff sprites
	getTexture(ResourceID::Texture_debuff_fear);
	getTexture(ResourceID::Texture_debuff_stun);
}



