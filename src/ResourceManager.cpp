
#include "ResourceManager.h"
#include "AnimatedSprite.h"

using namespace std;

ResourceManager *g_resourceManager;

ResourceManager::ResourceManager() : m_currentError({ ErrorID::VOID, "" }) {
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
		{ ResourceID::BitmapFont_default_8, "res/fonts/default_bitmap_font_8.png" },
		{ ResourceID::BitmapFont_default_12, "res/fonts/default_bitmap_font_12.png" },
		{ ResourceID::BitmapFont_shadowed_8, "res/fonts/shadowed_bitmap_font_8.png" },
		{ ResourceID::BitmapFont_shadowed_12, "res/fonts/shadowed_bitmap_font_12.png" },
		{ ResourceID::Configuration, "config.ini" },
		{ ResourceID::Save_folder, "saves/" },
		{ ResourceID::Npc_folder, "res/npc/" },
		{ ResourceID::Quicksave, "saves/quicksave.sav" },
		{ ResourceID::Items, "res/items.csv" },
		{ ResourceID::Texture_spellicons, "res/assets/spells/spritesheet_spellicons.png" },
		{ ResourceID::Texture_spellscroll, "res/assets/spells/scroll.png" },
		{ ResourceID::Texture_mapmarkers, "res/assets/misc/spritesheet_mapmarkers.png" },
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
		{ ResourceID::Texture_spell_projectile, "res/assets/spells/spritesheet_spell_projectile.png" },
		{ ResourceID::Texture_enemy_rat, "res/assets/enemies/spritesheet_enemy_rat.png" },
		{ ResourceID::Texture_enemy_firerat, "res/assets/enemies/spritesheet_enemy_firerat.png" },
		{ ResourceID::Texture_enemy_nekomata, "res/assets/enemies/spritesheet_enemy_nekomata.png" },
		{ ResourceID::Texture_enemy_crow, "res/assets/enemies/spritesheet_enemy_crow.png" },
		{ ResourceID::Texture_enemy_skeleton, "res/assets/enemies/spritesheet_enemy_skeleton.png" },
		{ ResourceID::Texture_enemy_gargoyle, "res/assets/enemies/spritesheet_enemy_gargoyle.png" },
		{ ResourceID::Texture_enemy_hunter, "res/assets/enemies/spritesheet_enemy_hunter.png" },
		{ ResourceID::Texture_enemy_wisp, "res/assets/enemies/spritesheet_enemy_wisp.png" },
		{ ResourceID::Texture_enemy_seagull, "res/assets/enemies/spritesheet_enemy_seagull.png" },
		{ ResourceID::Texture_enemy_wolf, "res/assets/enemies/spritesheet_enemy_wolf.png" },
		{ ResourceID::Texture_enemy_stoneman, "res/assets/enemies/spritesheet_enemy_stoneman.png" },
		{ ResourceID::Texture_tile_ice, "res/assets/level_dynamic_tiles/spritesheet_tiles_ice.png" },
		{ ResourceID::Texture_tile_destructible, "res/assets/level_dynamic_tiles/spritesheet_tiles_destructible.png" },
		{ ResourceID::Texture_tile_torch, "res/assets/level_dynamic_tiles/spritesheet_tiles_torch.png" },
		{ ResourceID::Texture_tile_chest, "res/assets/level_dynamic_tiles/spritesheet_tiles_chest.png" },
		{ ResourceID::Texture_tile_spikesbottom, "res/assets/level_dynamic_tiles/spritesheet_tiles_spikesbottom.png" },
		{ ResourceID::Texture_tile_spikestop, "res/assets/level_dynamic_tiles/spritesheet_tiles_spikestop.png" },
		{ ResourceID::Texture_tile_frozenwater, "res/assets/level_dynamic_tiles/spritesheet_tiles_frozenwater.png" },
		{ ResourceID::Texture_tile_shiftable, "res/assets/level_dynamic_tiles/spritesheet_tiles_shiftable.png" },
		{ ResourceID::Texture_tile_checkpoint, "res/assets/level_dynamic_tiles/spritesheet_tiles_checkpoint.png" },
		{ ResourceID::Texture_tile_lever, "res/assets/level_dynamic_tiles/spritesheet_tiles_lever.png" },
		{ ResourceID::Texture_tile_switchable, "res/assets/level_dynamic_tiles/spritesheet_tiles_switchable.png" },
		{ ResourceID::Texture_tile_cooking, "res/assets/map_dynamic_tiles/spritesheet_tiles_cooking.png" },
		{ ResourceID::Texture_tile_moving, "res/assets/level_dynamic_tiles/spritesheet_tiles_moving.png" },
		{ ResourceID::Texture_tile_waypoint, "res/assets/map_dynamic_tiles/spritesheet_tiles_waypoint.png" },
		{ ResourceID::Texture_tile_unstable, "res/assets/level_dynamic_tiles/spritesheet_tiles_unstable.png" },
		{ ResourceID::Texture_tile_falling, "res/assets/level_dynamic_tiles/spritesheet_tiles_falling.png" },
		{ ResourceID::Texture_tile_jumping, "res/assets/level_dynamic_tiles/spritesheet_tiles_jumping.png" },
		{ ResourceID::Texture_tile_shooting, "res/assets/level_dynamic_tiles/spritesheet_tiles_shooting.png" },
		{ ResourceID::Texture_tile_book, "res/assets/map_dynamic_tiles/spritesheet_tiles_book.png" },
		{ ResourceID::Texture_tile_sign_map, "res/assets/map_dynamic_tiles/spritesheet_tiles_sign.png" },
		{ ResourceID::Texture_tile_sign_level, "res/assets/level_dynamic_tiles/spritesheet_tiles_sign.png" },
		{ ResourceID::Texture_misc_cooking, "res/assets/misc/sprite_cooking.png" },
		{ ResourceID::Texture_screen_splash, "res/assets/screens/screen_splash.png" },
		{ ResourceID::Texture_screen_splash_fireanimation, "res/assets/misc/spritesheet_fireanimation.png" },
		{ ResourceID::Texture_screen_error_fileNotFound, "res/assets/screens/screen_error_filenotfound.png" },
		{ ResourceID::Texture_screen_error_dataCorrupted, "res/assets/screens/screen_error_datacorrupted.png" },
		{ ResourceID::Texture_screen_gameover, "res/assets/screens/screen_gameover.png" },
		{ ResourceID::Texture_screen_overlay, "res/assets/screens/screen_overlay.png" },
		{ ResourceID::Texture_screen_overlay_stunned, "res/assets/screens/screen_overlay_stunned.png" },
		{ ResourceID::Texture_screen_overlay_feared, "res/assets/screens/screen_overlay_feared.png" },
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
		{ ResourceID::Texture_GUI_window_close_button , "res/assets/gui/window_close_button.png" },
		{ ResourceID::Texture_GUI_arrow, "res/assets/gui/arrow.png" },
		{ ResourceID::Texture_GUI_arrow_left, "res/assets/gui/arrow_left.png" },
		{ ResourceID::Texture_GUI_arrow_right, "res/assets/gui/arrow_right.png" },
		{ ResourceID::Texture_GUI_exit_arrow, "res/assets/gui/exit_arrow.png" },
		{ ResourceID::Texture_GUI_checkbox, "res/assets/gui/checkbox.png" },
		{ ResourceID::Texture_GUI_slider, "res/assets/gui/slider.png" },
		{ ResourceID::Texture_GUI_knob, "res/assets/gui/knob.png" },
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
		{ ResourceID::Texture_GUI_tab_inactive, "res/assets/gui/tab_inactive.png" },
		{ ResourceID::Texture_GUI_tab_active, "res/assets/gui/tab_active.png" },
		{ ResourceID::Texture_GUI_cursor, "res/assets/gui/cursor.png" },
		{ ResourceID::Texture_GUI_speechbubble_pointer, "res/assets/gui/speechbubble_pointer.png" },
		{ ResourceID::Texture_GUI_healthbar_border, "res/assets/gui/healthbar_border.png" },
		{ ResourceID::Texture_GUI_healthbar_content, "res/assets/gui/healthbar_content.png" },
		{ ResourceID::Texture_GUI_healthbar_content_hit, "res/assets/gui/healthbar_content_hit.png" },
		{ ResourceID::Texture_GUI_healthbar_content_highlight, "res/assets/gui/healthbar_content_highlight.png" },
		{ ResourceID::Texture_Particle_circle, "res/assets/particles/circle.png" },
		{ ResourceID::Texture_Particle_blob, "res/assets/particles/blob.png" },
		{ ResourceID::Texture_Particle_blob2, "res/assets/particles/blob2.png" },
		{ ResourceID::Texture_Particle_longblob, "res/assets/particles/longblob.png" },
		{ ResourceID::Texture_Particle_snowflake, "res/assets/particles/snowflake.png" },
		{ ResourceID::Texture_Particle_smoke, "res/assets/particles/smoke.png" },
		{ ResourceID::Texture_Particle_flame, "res/assets/particles/flame.png" },
		{ ResourceID::Texture_Particle_star, "res/assets/particles/star.png" },
		{ ResourceID::Texture_Particle_rain, "res/assets/particles/rain.png" },
		{ ResourceID::Texture_Particle_snow, "res/assets/particles/snow.png" },
		{ ResourceID::Sound_spell_fireball, "res/sound/spell/fireball.ogg" },
		{ ResourceID::Sound_tile_water, "res/sound/tile/water_splash.ogg" },
		{ ResourceID::Sound_tile_lever, "res/sound/tile/lever_click.ogg" },
		{ ResourceID::Sound_tile_waypoint, "res/sound/tile/teleport.ogg" },
		{ ResourceID::Sound_tile_checkpoint, "res/sound/tile/gargoyle.ogg" },
		{ ResourceID::Sound_tile_destructible, "res/sound/tile/crumble.ogg" },
		{ ResourceID::Sound_gui_turnpage, "res/sound/gui/page_turn.ogg" },
		{ ResourceID::Sound_gui_menucursor, "res/sound/gui/menu_cursor.ogg" },
		{ ResourceID::Sound_gui_openwindow, "res/sound/gui/window_open.ogg" },
		{ ResourceID::Sound_item_gold, "res/sound/item/pickup_gold.ogg" },
		{ ResourceID::Sound_cendric_death, "res/sound/mob/cendric_death.ogg" },
	});

	// fonts should be always loaded to avoid lags when loading later
	getBitmapFont(ResourceID::BitmapFont_default_8);
	getBitmapFont(ResourceID::BitmapFont_default_12);
	getBitmapFont(ResourceID::BitmapFont_shadowed_8);
	getBitmapFont(ResourceID::BitmapFont_shadowed_12);

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
	if (texture->loadFromFile(getPath(filename))) {
		m_textures[filename] = texture;
		g_logger->logInfo("ResourceManager", getPath(std::string(filename)) + ": loading texture");
		return m_textures[filename];
	}

	g_logger->logError("ResourceManager", "Texture could not be loaded from file: " + getPath(std::string(filename)));
	std::string tmp = "Texture could not be loaded from file: " + getPath(filename);
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
	if (soundBuffer->loadFromFile(getPath(filename))) {
		m_soundBuffers[filename] = soundBuffer;
		g_logger->logInfo("ResourceManager", getPath(std::string(filename)) + ": loading soundbuffer");
		return m_soundBuffers[filename];
	}

	g_logger->logError("ResourceManager", "Soundbuffer could not be loaded from file: " + getPath(std::string(filename)));
	std::string tmp = "Soundbuffer could not be loaded from file: " + getPath(filename);
	setError(ErrorID::Error_fileNotFound, tmp);
	m_soundBuffers[filename] = soundBuffer;
	return m_soundBuffers[filename];
}

sf::SoundBuffer* ResourceManager::getSoundBuffer(ResourceID id) {
	return getSoundBuffer(m_fileNames[id]);
}

sf::Font* ResourceManager::getFont(const std::string& filename) {
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
	if (font->loadFromFile(getPath(filename))) {
		m_fonts[filename] = font;
		g_logger->logInfo("ResourceManager", getPath(std::string(filename)) + ": loading font");
		return m_fonts[filename];
	}

	g_logger->logError("ResourceManager", "Font could not be loaded from file: " + getPath(std::string(filename)));
	std::string tmp = "Font could not be loaded from file: " + getPath(filename);
	setError(ErrorID::Error_fileNotFound, tmp);
	m_fonts[filename] = font;
	return m_fonts[filename];
}

sf::Font* ResourceManager::getFont(ResourceID id) {
	return getFont(m_fileNames[id]);
}

BitmapFont* ResourceManager::getBitmapFont(const std::string& filename) {
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
	if (font->loadFromFile(getPath(filename))) {
		m_bitmapFonts[filename] = font;
		g_logger->logInfo("ResourceManager", getPath(std::string(filename)) + ": loading bitmap font");
		return m_bitmapFonts[filename];
	}

	g_logger->logError("ResourceManager", "Bitmap Font could not be loaded from file: " + getPath(std::string(filename)));
	std::string tmp = "Bitmap Font could not be loaded from file: " + getPath(filename);
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

void ResourceManager::deleteResource(const std::string& filename) {
	// delete texture
	auto const &textureIt = m_textures.find(filename);
	if (textureIt != m_textures.end()) {
		delete textureIt->second;
		m_textures.erase(textureIt);
		g_logger->logInfo("ResourceManager", getPath(std::string(filename)) + ": releasing texture");
		return;
	}

	// delete font
	auto const &fontIt = m_fonts.find(filename);
	if (fontIt != m_fonts.end()) {
		delete fontIt->second;
		m_fonts.erase(fontIt);
		g_logger->logInfo("ResourceManager", getPath(std::string(filename)) + ": releasing font");
		return;
	}

	// delete bitmap font
	auto const &bitmapFontIt = m_bitmapFonts.find(filename);
	if (bitmapFontIt != m_bitmapFonts.end()) {
		delete bitmapFontIt->second;
		m_bitmapFonts.erase(bitmapFontIt);
		g_logger->logInfo("ResourceManager", getPath(std::string(filename)) + ": releasing bitmap font");
		return;
	}

	// delete soundbuffer
	auto const &soundBufferIt = m_soundBuffers.find(filename);
	if (soundBufferIt != m_soundBuffers.end()) {
		delete soundBufferIt->second;
		m_soundBuffers.erase(soundBufferIt);
		g_logger->logInfo("ResourceManager", getPath(std::string(filename)) + ": releasing soundbuffer");
		return;
	}
}

void ResourceManager::playSound(sf::Sound& sound, ResourceID id, bool force, float scale) {
	if (id == ResourceID::VOID) return;
	// don't play the sound if it's already playing and we're not forcing
	if (!force && sound.getStatus() == sf::SoundSource::Status::Playing) return;
	if (m_configuration.isSoundOn) {
		sound.setBuffer(*getSoundBuffer(id));
		scale = clamp(scale, 0.f, 1.f);
		sound.setVolume(static_cast<float>(m_configuration.volumeSound) * scale);
		sound.play();
	}
}

void ResourceManager::playMusic(const std::string& filename, bool looping, const sf::Time& playingOffset) {
	if (!m_configuration.isSoundOn || filename.empty()) return;
	if (m_currentMusic.first.compare(filename) == 0) return; // already playing
	m_currentMusic.second.stop();
	if (filename.empty()) return;
	if (m_currentMusic.second.openFromFile(getPath(filename))) {
		m_currentMusic.second.setLoop(looping);
		m_currentMusic.second.setVolume(static_cast<float>(m_configuration.volumeMusic));
		m_currentMusic.second.play();
		if (playingOffset < m_currentMusic.second.getDuration())
			m_currentMusic.second.setPlayingOffset(playingOffset);
	}
	else {
		g_logger->logError("ResourceManager", "Could not read music from file: " + getPath(filename));
	}
}

void ResourceManager::stopMusic() {
	m_currentMusic.second.stop();
	m_currentMusic.first.clear();
}

const std::pair<ErrorID, std::string>* ResourceManager::pollError() const {
	return &m_currentError;
}

void ResourceManager::setError(ErrorID id, const string &description) {
	m_currentError.first = id;
	m_currentError.second = description;
}

std::string ResourceManager::getFilename(ResourceID id) {
	return getPath(m_fileNames[id]);
}

ConfigurationData& ResourceManager::getConfiguration() {
	return m_configuration;
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
	deleteResource(ResourceID::Texture_spell_projectile);

	deleteResource(ResourceID::Sound_spell_fireball);

	// delete particle resources for spells
	deleteResource(ResourceID::Texture_Particle_snowflake);
	deleteResource(ResourceID::Texture_Particle_smoke);
	deleteResource(ResourceID::Texture_Particle_flame);
	deleteResource(ResourceID::Texture_Particle_star);

	// delete dynamic tile resources
	deleteResource(ResourceID::Texture_tile_frozenwater);
	deleteResource(ResourceID::Texture_tile_ice);
	deleteResource(ResourceID::Texture_tile_destructible);
	deleteResource(ResourceID::Texture_tile_torch);
	deleteResource(ResourceID::Texture_tile_chest);
	deleteResource(ResourceID::Texture_tile_spikesbottom);
	deleteResource(ResourceID::Texture_tile_spikestop);
	deleteResource(ResourceID::Texture_tile_shiftable);
	deleteResource(ResourceID::Texture_tile_checkpoint);
	deleteResource(ResourceID::Texture_tile_lever);
	deleteResource(ResourceID::Texture_tile_switchable);
	deleteResource(ResourceID::Texture_tile_moving);
	deleteResource(ResourceID::Texture_tile_unstable);
	deleteResource(ResourceID::Texture_tile_falling);
	deleteResource(ResourceID::Texture_tile_jumping);
	deleteResource(ResourceID::Texture_tile_shooting);
	deleteResource(ResourceID::Texture_tile_sign_level);

	deleteResource(ResourceID::Sound_tile_water);
	deleteResource(ResourceID::Sound_tile_lever);
	deleteResource(ResourceID::Sound_tile_checkpoint);
	deleteResource(ResourceID::Sound_tile_destructible);

	// delete enemy resources
	deleteResource(ResourceID::Texture_enemy_rat);
	deleteResource(ResourceID::Texture_enemy_firerat);
	deleteResource(ResourceID::Texture_enemy_nekomata);
	deleteResource(ResourceID::Texture_enemy_crow);
	deleteResource(ResourceID::Texture_enemy_skeleton);
	deleteResource(ResourceID::Texture_enemy_gargoyle);
	deleteResource(ResourceID::Texture_enemy_hunter);
	deleteResource(ResourceID::Texture_enemy_wisp);
	deleteResource(ResourceID::Texture_enemy_seagull);
	deleteResource(ResourceID::Texture_enemy_wolf);

	// delete debuff resources
	deleteResource(ResourceID::Texture_debuff_fear);
	deleteResource(ResourceID::Texture_debuff_stun);

	// delete other level resources
	deleteResource(ResourceID::Texture_levelitems);
	deleteResource(ResourceID::Texture_screen_gameover);
	deleteResource(ResourceID::Texture_screen_overlay_stunned);
	deleteResource(ResourceID::Texture_screen_overlay_feared);
	deleteResource(ResourceID::Texture_GUI_speechbubble_pointer);
}

void ResourceManager::loadMapResources() {
	getTexture(ResourceID::Texture_dialogue);
	getTexture(ResourceID::Texture_mapmarkers);
}

void ResourceManager::deleteMapResources() {
	deleteResource(ResourceID::Texture_dialogue);
	deleteResource(ResourceID::Texture_mapmarkers);

	// dynamic tile resources
	deleteResource(ResourceID::Texture_tile_cooking);
	deleteResource(ResourceID::Texture_tile_waypoint);
	deleteResource(ResourceID::Texture_tile_book);
	deleteResource(ResourceID::Texture_tile_sign_map);

	deleteResource(ResourceID::Sound_tile_waypoint);
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
	getTexture(ResourceID::Texture_enemy_gargoyle);
	getTexture(ResourceID::Texture_spell_projectile);
	getTexture(ResourceID::Texture_tile_frozenwater);

	getSoundBuffer(ResourceID::Sound_spell_fireball);

	// load particle resources for spells
	getTexture(ResourceID::Texture_Particle_snowflake);
	getTexture(ResourceID::Texture_Particle_smoke);
	getTexture(ResourceID::Texture_Particle_blob2);
	getTexture(ResourceID::Texture_Particle_flame);
	getTexture(ResourceID::Texture_Particle_star);

	// get sounds
	getSoundBuffer(ResourceID::Sound_tile_water);
	getSoundBuffer(ResourceID::Sound_tile_lever);
	getSoundBuffer(ResourceID::Sound_tile_checkpoint);
	getSoundBuffer(ResourceID::Sound_tile_destructible);

	// load overlays
	getTexture(ResourceID::Texture_screen_gameover);
	getTexture(ResourceID::Texture_screen_overlay_stunned);
	getTexture(ResourceID::Texture_screen_overlay_feared);

	// load debuff sprites
	getTexture(ResourceID::Texture_debuff_fear);
	getTexture(ResourceID::Texture_debuff_stun);
}



