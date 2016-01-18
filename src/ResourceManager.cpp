#include "ResourceManager.h"
#include "AnimatedSprite.h"

using namespace std;

ResourceManager *g_resourceManager;

ResourceManager::ResourceManager() : m_currentError(ErrorID::VOID, "") {
}

ResourceManager::~ResourceManager() {
	for (auto& it : m_textures) {
		delete it.second;
	}
	for (auto& it : m_soundBuffers) {
		delete it.second;
	}
	for (auto& it : m_fonts) {
		delete it.second;
	}
	for (auto& it : m_bitmapFonts) {
		delete it.second;
	}
	m_textures.clear();
	m_soundBuffers.clear();
	m_fonts.clear();
	m_bitmapFonts.clear();
	m_fileNames.clear();
}

void ResourceManager::init() {
	m_fileNames.insert(
	{
		{ ResourceID::BitmapFont_default, "res/fonts/default_bitmap_font.png" },
		{ ResourceID::Configuration, "config.ini" },
		{ ResourceID::Save_folder, "saves/" },
		{ ResourceID::Npc_folder, "res/npc/" },
		{ ResourceID::Quicksave, "saves/quicksave.sav" },
		{ ResourceID::Items, "res/items.csv" },
		{ ResourceID::Texture_spellicons, "res/assets/spells/spritesheet_spellicons.png" },
		{ ResourceID::Texture_damageTypes, "res/assets/debuffs/spritesheet_damagetypes.png" },
		{ ResourceID::Texture_mainChar, "res/assets/cendric/spritesheet_cendric_level.png" },
		{ ResourceID::Texture_mapMainChar, "res/assets/cendric/spritesheet_cendric_map.png" },
		{ ResourceID::Texture_spell_fireball, "res/assets/spells/spritesheet_spell_fireball.png" },
		{ ResourceID::Texture_spell_divineshield, "res/assets/spells/spritesheet_spell_divineshield.png" },
		{ ResourceID::Texture_spell_iceball, "res/assets/spells/spritesheet_spell_iceball.png" },
		{ ResourceID::Texture_spell_aureola, "res/assets/spells/spritesheet_spell_aureola.png" },
		{ ResourceID::Texture_spell_fear, "res/assets/spells/spritesheet_spell_fear.png" },
		{ ResourceID::Texture_spell_icyambush, "res/assets/spells/spritesheet_spell_icyambush.png" },
		{ ResourceID::Texture_spell_unlock, "res/assets/spells/spritesheet_spell_unlock.png" },
		{ ResourceID::Texture_spell_telekinesis, "res/assets/spells/spritesheet_spell_telekinesis.png" },
		{ ResourceID::Texture_spell_flash, "res/assets/spells/spritesheet_spell_flash.png" },
		{ ResourceID::Texture_spell_leapoffaith, "res/assets/spells/spritesheet_spell_leapoffaith.png" },
		{ ResourceID::Texture_spell_ghost, "res/assets/spells/spritesheet_spell_ghost.png" },
		{ ResourceID::Texture_spell_shadowtrap, "res/assets/spells/spritesheet_spell_shadowtrap.png" },
		{ ResourceID::Texture_enemy_rat, "res/assets/enemies/spritesheet_enemy_rat.png" },
		{ ResourceID::Texture_enemy_firerat, "res/assets/enemies/spritesheet_enemy_firerat.png" },
		{ ResourceID::Texture_enemy_nekomata_blue_trans, "res/assets/enemies/spritesheet_enemy_nekomata_blue_trans.png" },
		{ ResourceID::Texture_enemy_crow, "res/assets/enemies/spritesheet_enemy_crow.png" },
		{ ResourceID::Texture_enemy_skeleton, "res/assets/enemies/spritesheet_enemy_skeleton.png" },
		{ ResourceID::Texture_tile_ice, "res/assets/level_dynamic_tiles/spritesheet_tiles_ice.png" },
		{ ResourceID::Texture_tile_crumblyblock, "res/assets/level_dynamic_tiles/spritesheet_tiles_crumblyblock.png" },
		{ ResourceID::Texture_tile_torch, "res/assets/level_dynamic_tiles/spritesheet_tiles_torch.png" },
		{ ResourceID::Texture_tile_chest, "res/assets/level_dynamic_tiles/spritesheet_tiles_chest.png" },
		{ ResourceID::Texture_tile_spikesbottom, "res/assets/level_dynamic_tiles/spritesheet_tiles_spikesbottom.png" },
		{ ResourceID::Texture_tile_spikestop, "res/assets/level_dynamic_tiles/spritesheet_tiles_spikestop.png" },
		{ ResourceID::Texture_tile_frozenwater, "res/assets/level_dynamic_tiles/spritesheet_tiles_frozenwater.png" },
		{ ResourceID::Texture_tile_shiftableblock, "res/assets/level_dynamic_tiles/spritesheet_tiles_shiftableblock.png" },
		{ ResourceID::Texture_tile_checkpoint, "res/assets/level_dynamic_tiles/spritesheet_tiles_checkpoint.png" },
		{ ResourceID::Texture_tile_lever, "res/assets/level_dynamic_tiles/spritesheet_tiles_lever.png" },
		{ ResourceID::Texture_tile_switchable, "res/assets/level_dynamic_tiles/spritesheet_tiles_switchable.png" },
		{ ResourceID::Texture_tile_cooking, "res/assets/map_dynamic_tiles/spritesheet_tiles_cooking.png" },
		{ ResourceID::Texture_misc_cooking, "res/assets/misc/sprite_cooking.png" },
		{ ResourceID::Texture_screen_splash, "res/assets/screens/screen_splash.png" },
		{ ResourceID::Texture_screen_splash_fireanimation, "res/assets/misc/spritesheet_fireanimation.png" },
		{ ResourceID::Texture_screen_error_fileNotFound, "res/assets/screens/screen_error_filenotfound.png" },
		{ ResourceID::Texture_screen_error_dataCorrupted, "res/assets/screens/screen_error_datacorrupted.png" },
		{ ResourceID::Texture_screen_gameover, "res/assets/screens/screen_gameover.png" },
		{ ResourceID::Texture_screen_overlay, "res/assets/screens/screen_overlay.png" },
		{ ResourceID::Texture_screen_menu, "res/assets/screens/screen_menu.png" },
		{ ResourceID::Texture_debuff_fear, "res/assets/debuffs/spritesheet_debuff_fear.png" },
		{ ResourceID::Texture_debuff_stun, "res/assets/debuffs/spritesheet_debuff_stun.png" },
		{ ResourceID::Texture_screen_credits, "res/assets/screens/screen_credits.png" },
		{ ResourceID::Texture_levelitems, "res/assets/items/spritesheet_levelitems.png" },
		{ ResourceID::Texture_items, "res/assets/items/spritesheet_items.png" },
		{ ResourceID::Texture_dialogue, "res/assets/dialogue/spritesheet_dialogue.png" },
		{ ResourceID::Texture_inventorytabs, "res/assets/misc/spritesheet_inventorytabs.png" },
		{ ResourceID::Texture_equipmentplaceholders, "res/assets/misc/spritesheet_equipmentplaceholders.png" },
		{ ResourceID::Texture_gems, "res/assets/misc/spritesheet_gems.png" },
		{ ResourceID::Texture_GUI_rounded_rectangle, "res/assets/gui/rounded_rectangle.png" },
		{ ResourceID::Texture_GUI_ornament_none, "res/assets/gui/ornament_none.png" },
		{ ResourceID::Texture_GUI_ornament_small, "res/assets/gui/ornament_small.png" },
		{ ResourceID::Texture_GUI_ornament_medium, "res/assets/gui/ornament_medium.png" },
		{ ResourceID::Texture_GUI_ornament_large, "res/assets/gui/ornament_large.png" },
		{ ResourceID::Texture_GUI_window_ornament, "res/assets/gui/window_ornament.png" },
		{ ResourceID::Texture_GUI_arrow, "res/assets/gui/arrow.png" },
		{ ResourceID::Texture_GUI_spell_color_elemental, "res/assets/gui/spell_color_elemental.png" },
		{ ResourceID::Texture_GUI_spell_color_twilight, "res/assets/gui/spell_color_twilight.png" },
		{ ResourceID::Texture_GUI_spell_color_necromancy, "res/assets/gui/spell_color_necromancy.png" },
		{ ResourceID::Texture_GUI_spell_color_divine, "res/assets/gui/spell_color_divine.png" },
		{ ResourceID::Texture_GUI_spell_color_illusion, "res/assets/gui/spell_color_illusion.png" },
		{ ResourceID::Texture_GUI_slot_inventory, "res/assets/gui/slot_inventory.png" },
		{ ResourceID::Texture_GUI_slot_inventory_selected, "res/assets/gui/slot_inventory_selected.png" },
		{ ResourceID::Texture_GUI_slot_modifier, "res/assets/gui/slot_modifier.png" },
		{ ResourceID::Texture_GUI_slot_modifier_selected, "res/assets/gui/slot_modifier_selected.png" },
		{ ResourceID::Texture_GUI_slot_spell, "res/assets/gui/slot_spell.png" },
		{ ResourceID::Texture_GUI_slot_spell_selected, "res/assets/gui/slot_spell_selected.png" },
		{ ResourceID::Texture_GUI_slot_spell_gem, "res/assets/gui/slot_spell_gem.png" },
		{ ResourceID::Texture_GUI_slot_highlight, "res/assets/gui/slot_highlight.png" },
		{ ResourceID::Texture_GUI_slot_buff, "res/assets/gui/slot_buff.png" },
		{ ResourceID::Texture_Particle_circle, "res/assets/particles/circle.png" },
		{ ResourceID::Texture_Particle_blob, "res/assets/particles/blob.png" },
		{ ResourceID::Texture_Particle_blob2, "res/assets/particles/blob2.png" },
		{ ResourceID::Texture_Particle_longblob, "res/assets/particles/longblob.png" },
		{ ResourceID::Texture_Particle_snowflake, "res/assets/particles/snowflake.png" },
		{ ResourceID::Texture_Particle_smoke, "res/assets/particles/smoke.png" },
		{ ResourceID::Sound_spell_fireball, "res/sound/sound_spell_fireball.wav" },
		{ ResourceID::Sound_tile_water, "res/sound/sound_tile_water.wav" },
		{ ResourceID::Sound_tile_lever, "res/sound/sound_tile_lever.ogg" }
	});

	// font should be always loaded to avoid lags when loading later
	getBitmapFont(ResourceID::BitmapFont_default);

	ConfigurationReader reader;
	if (!reader.readConfiguration(m_configuration)) {
		m_configuration = DEFAULT_CONFIGURATION;
	}
}

sf::Texture* ResourceManager::getTexture(const std::string& filename) {
	// does the texture exist yet?
	for (std::map<std::string, sf::Texture*>::iterator it = m_textures.begin();
		it != m_textures.end();
		++it) {
		if (filename.compare(it->first) == 0) {
			return it->second;
		}
	}

	// the texture doesn't exist. Create and save it.
	sf::Texture* texture = new sf::Texture();

	// search project's main directory
	if (texture->loadFromFile(filename)) {
		m_textures[filename] = texture;
		g_logger->logInfo("ResourceManager", std::string(filename) + ": loading texture");
		return m_textures[filename];
	}

	g_logger->logError("ResourceManager", "Texture could not be loaded from file: " + std::string(filename));
	std::string tmp = "Texture could not be loaded from file: " + filename;
	setError(ErrorID::Error_fileNotFound, tmp);
	m_textures[filename] = texture;
	return m_textures[filename];
}

sf::Texture* ResourceManager::getTexture(ResourceID id) {
	return getTexture(m_fileNames[id]);
}

sf::SoundBuffer* ResourceManager::getSoundBuffer(const std::string& filename) {
	// does the soundbuffer exist yet?
	for (std::map<std::string, sf::SoundBuffer*>::iterator it = m_soundBuffers.begin();
		it != m_soundBuffers.end();
		++it) {
		if (filename.compare(it->first) == 0) {
			return it->second;
		}
	}

	// the soundbuffer doesn't exist. Create and save it.
	sf::SoundBuffer* soundBuffer = new sf::SoundBuffer();

	// search project's main directory
	if (soundBuffer->loadFromFile(filename)) {
		m_soundBuffers[filename] = soundBuffer;
		g_logger->logInfo("ResourceManager", std::string(filename) + ": loading soundbuffer");
		return m_soundBuffers[filename];
	}

	g_logger->logError("ResourceManager", "Soundbuffer could not be loaded from file: " + std::string(filename));
	std::string tmp = "Soundbuffer could not be loaded from file: " + filename;
	setError(ErrorID::Error_fileNotFound, tmp);
	m_soundBuffers[filename] = soundBuffer;
	return m_soundBuffers[filename];
}

sf::SoundBuffer* ResourceManager::getSoundBuffer(ResourceID id) {
	return getSoundBuffer(m_fileNames[id]);
}

sf::Font* ResourceManager::getFont(const std::string &filename) {
	// does the font exist yet?
	for (std::map<std::string, sf::Font*>::iterator it = m_fonts.begin();
		it != m_fonts.end();
		++it) {
		if (filename.compare(it->first) == 0) {
			return it->second;
		}
	}

	// the font doesn't exist. Create and save it.
	sf::Font* font = new sf::Font();

	// search project's main directory
	if (font->loadFromFile(filename)) {
		m_fonts[filename] = font;
		g_logger->logInfo("ResourceManager", std::string(filename) + ": loading font");
		return m_fonts[filename];
	}

	g_logger->logError("ResourceManager", "Font could not be loaded from file: " + std::string(filename));
	std::string tmp = "Font could not be loaded from file: " + filename;
	setError(ErrorID::Error_fileNotFound, tmp);
	m_fonts[filename] = font;
	return m_fonts[filename];
}

sf::Font* ResourceManager::getFont(ResourceID id) {
	return getFont(m_fileNames[id]);
}

BitmapFont* ResourceManager::getBitmapFont(const std::string &filename) {
	// does the font exist yet?
	for (auto it = m_bitmapFonts.begin();
		it != m_bitmapFonts.end();
		++it) {
		if (filename.compare(it->first) == 0) {
			return it->second;
		}
	}

	// the font doesn't exist. Create and save it.
	BitmapFont* font = new BitmapFont();

	// search project's main directory
	if (font->loadFromFile(filename)) {
		m_bitmapFonts[filename] = font;
		g_logger->logInfo("ResourceManager", std::string(filename) + ": loading bitmap font");
		return m_bitmapFonts[filename];
	}

	g_logger->logError("ResourceManager", "Bitmap Font could not be loaded from file: " + std::string(filename));
	std::string tmp = "Bitmap Font could not be loaded from file: " + filename;
	setError(ErrorID::Error_fileNotFound, tmp);
	m_bitmapFonts[filename] = font;
	return m_bitmapFonts[filename];
}

BitmapFont* ResourceManager::getBitmapFont(ResourceID id) {
	return getBitmapFont(m_fileNames[id]);
}

void ResourceManager::deleteResource(ResourceID id) {
	deleteResource(m_fileNames[id]);
}

void ResourceManager::deleteResource(const std::string &filename) {
	// delete texture
	auto const &textureIt = m_textures.find(filename);
	if (textureIt != m_textures.end()) {
		delete textureIt->second;
		m_textures.erase(textureIt);
		g_logger->logInfo("ResourceManager", std::string(filename) + ": releasing texture");
		return;
	}

	// delete font
	auto const &fontIt = m_fonts.find(filename);
	if (fontIt != m_fonts.end()) {
		delete fontIt->second;
		m_fonts.erase(fontIt);
		g_logger->logInfo("ResourceManager", std::string(filename) + ": releasing font");
		return;
	}

	// delete bitmap font
	auto const &bitmapFontIt = m_bitmapFonts.find(filename);
	if (bitmapFontIt != m_bitmapFonts.end()) {
		delete bitmapFontIt->second;
		m_bitmapFonts.erase(bitmapFontIt);
		g_logger->logInfo("ResourceManager", std::string(filename) + ": releasing bitmap font");
		return;
	}

	// delete soundbuffer
	auto const &soundBufferIt = m_soundBuffers.find(filename);
	if (soundBufferIt != m_soundBuffers.end()) {
		delete soundBufferIt->second;
		m_soundBuffers.erase(soundBufferIt);
		g_logger->logInfo("ResourceManager", std::string(filename) + ": releasing soundbuffer");
		return;
	}
}

void ResourceManager::playSound(sf::Sound& sound, ResourceID id) {
	if (id == ResourceID::VOID) return;
	// don't play the sound if it's already playing
	if (sound.getStatus() == sf::SoundSource::Status::Playing) return;
	if (m_configuration.isSoundOn) {
		sound.setBuffer(*getSoundBuffer(id));
		sound.setVolume(static_cast<float>(m_configuration.volumeSound));
		sound.play();
	}
}

void ResourceManager::playMusic(sf::Music& music, const std::string& filename) {
	if (m_configuration.isSoundOn && !filename.empty()) {
		if (music.openFromFile(filename)) {
			music.setLoop(true);
			music.setVolume(static_cast<float>(m_configuration.volumeMusic));
			music.play();
		}
		else {
			g_logger->logError("ResourceManager", "Could not read music from file: " + filename);
		}
	}
}


char* ResourceManager::getFilename(ResourceID id) {
	return &m_fileNames[id][0u];
}

const std::pair<ErrorID, std::string>* ResourceManager::pollError() const {
	return &m_currentError;
}

ConfigurationData& ResourceManager::getConfiguration() {
	return m_configuration;
}

void ResourceManager::setError(ErrorID id, const string &description) {
	m_currentError.first = id;
	m_currentError.second = description;
}

void ResourceManager::deleteLevelResources() {
	// delete spell resources
	deleteResource(ResourceID::Texture_spell_fireball);
	deleteResource(ResourceID::Texture_spell_iceball);
	deleteResource(ResourceID::Texture_spell_divineshield);
	deleteResource(ResourceID::Texture_spell_aureola);
	deleteResource(ResourceID::Texture_spell_fear);
	deleteResource(ResourceID::Texture_spell_icyambush);
	deleteResource(ResourceID::Texture_spell_unlock);
	deleteResource(ResourceID::Texture_spell_flash);
	deleteResource(ResourceID::Texture_spell_leapoffaith);
	deleteResource(ResourceID::Texture_spell_ghost);
	deleteResource(ResourceID::Texture_spell_shadowtrap);

	deleteResource(ResourceID::Sound_spell_fireball);

	// delete particle resources for spells
	deleteResource(ResourceID::Texture_Particle_snowflake);
	deleteResource(ResourceID::Texture_Particle_smoke);

	// delete dynamic tile resources
	deleteResource(ResourceID::Texture_tile_frozenwater);
	deleteResource(ResourceID::Texture_tile_ice);
	deleteResource(ResourceID::Texture_tile_crumblyblock);
	deleteResource(ResourceID::Texture_tile_torch);
	deleteResource(ResourceID::Texture_tile_chest);
	deleteResource(ResourceID::Texture_tile_spikesbottom);
	deleteResource(ResourceID::Texture_tile_spikestop);
	deleteResource(ResourceID::Texture_tile_shiftableblock);
	deleteResource(ResourceID::Texture_tile_checkpoint);
	deleteResource(ResourceID::Texture_tile_lever);
	deleteResource(ResourceID::Texture_tile_switchable);

	deleteResource(ResourceID::Sound_tile_water);
	deleteResource(ResourceID::Sound_tile_lever);

	// delete enemy resources
	deleteResource(ResourceID::Texture_enemy_rat);
	deleteResource(ResourceID::Texture_enemy_firerat);
	deleteResource(ResourceID::Texture_enemy_nekomata_blue_trans);
	deleteResource(ResourceID::Texture_enemy_crow);
	deleteResource(ResourceID::Texture_enemy_skeleton);

	// delete debuff resources
	deleteResource(ResourceID::Texture_debuff_fear);
	deleteResource(ResourceID::Texture_debuff_stun);

	// delete item in level resources
	deleteResource(ResourceID::Texture_levelitems);

	// delete game over sprite
	deleteResource(ResourceID::Texture_screen_gameover);

}

void ResourceManager::loadLevelResources() {
	// load spell resources
	getTexture(ResourceID::Texture_spell_fireball);
	getTexture(ResourceID::Texture_spell_iceball);
	getTexture(ResourceID::Texture_spell_divineshield);
	getTexture(ResourceID::Texture_spell_aureola);
	getTexture(ResourceID::Texture_spell_fear);
	getTexture(ResourceID::Texture_spell_icyambush);
	getTexture(ResourceID::Texture_spell_unlock);
	getTexture(ResourceID::Texture_spell_flash);
	getTexture(ResourceID::Texture_spell_leapoffaith);
	getTexture(ResourceID::Texture_spell_ghost);
	getTexture(ResourceID::Texture_spell_shadowtrap);

	getSoundBuffer(ResourceID::Sound_spell_fireball);

	// load particle resources for spells
	getTexture(ResourceID::Texture_Particle_snowflake);
	getTexture(ResourceID::Texture_Particle_smoke);
	getTexture(ResourceID::Texture_Particle_blob2);

	// load dynamic tile resources
	getTexture(ResourceID::Texture_tile_frozenwater);
	getTexture(ResourceID::Texture_tile_ice);
	getTexture(ResourceID::Texture_tile_crumblyblock);
	getTexture(ResourceID::Texture_tile_torch);
	getTexture(ResourceID::Texture_tile_chest);
	getTexture(ResourceID::Texture_tile_spikestop);
	getTexture(ResourceID::Texture_tile_spikesbottom);
	getTexture(ResourceID::Texture_tile_shiftableblock);
	getTexture(ResourceID::Texture_tile_checkpoint);
	getTexture(ResourceID::Texture_tile_lever);
	getTexture(ResourceID::Texture_tile_switchable);

	getSoundBuffer(ResourceID::Sound_tile_water);
	getSoundBuffer(ResourceID::Sound_tile_lever);

	// load game over sprite 
	getTexture(ResourceID::Texture_screen_gameover);

	// load debuff sprites
	getTexture(ResourceID::Texture_debuff_fear);
	getTexture(ResourceID::Texture_debuff_stun);
}



