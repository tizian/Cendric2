#include "ResourceManager.h"
#include "AnimatedSprite.h"
#include "GlobalResource.h"

using namespace std;

ResourceManager* g_resourceManager;

ResourceManager::ResourceManager() : m_currentError({ ErrorID::VOID, "" }) {
	init();
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
	m_resourceOwners.clear();
	m_levelResources.clear();
	m_mapResources.clear();
}

void ResourceManager::init() {
	// load global resources
	loadBitmapFont(GlobalResource::FONT_8, ResourceType::Global);
	loadBitmapFont(GlobalResource::FONT_8_SHADOWED, ResourceType::Global);
	loadBitmapFont(GlobalResource::FONT_12, ResourceType::Global);
	loadBitmapFont(GlobalResource::FONT_12_SHADOWED, ResourceType::Global);

	// load particle resources
	loadTexture(GlobalResource::TEX_PARTICLE_CIRCLE, ResourceType::Global);
	loadTexture(GlobalResource::TEX_PARTICLE_BLOB, ResourceType::Global);
	loadTexture(GlobalResource::TEX_PARTICLE_LONGBLOB, ResourceType::Global);
	loadTexture(GlobalResource::TEX_PARTICLE_LIGHT, ResourceType::Global);
	loadTexture(GlobalResource::TEX_PARTICLE_SNOWFLAKE, ResourceType::Global);
	loadTexture(GlobalResource::TEX_PARTICLE_SMOKE, ResourceType::Global);
	loadTexture(GlobalResource::TEX_PARTICLE_RAIN, ResourceType::Global);
	loadTexture(GlobalResource::TEX_PARTICLE_STAR, ResourceType::Global);
	loadTexture(GlobalResource::TEX_PARTICLE_SNOW, ResourceType::Global);
	loadTexture(GlobalResource::TEX_PARTICLE_FLAME, ResourceType::Global);

	// load global miscanellous resources
	loadTexture(GlobalResource::TEX_ITEMS, ResourceType::Global);
	loadTexture(GlobalResource::TEX_LEVELITEMS, ResourceType::Global);
	loadTexture(GlobalResource::TEX_SPELLICONS, ResourceType::Global);
	loadTexture(GlobalResource::TEX_SPELLSCROLL, ResourceType::Global);
	loadTexture(GlobalResource::TEX_INVENTORYTABS, ResourceType::Global);
	loadTexture(GlobalResource::TEX_EQUIPMENTPLACEHOLDERS, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GEMS, ResourceType::Global);

	// load gui resources
	loadTexture(GlobalResource::TEX_GUI_ROUNDED_RECTANGLE, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_ORNAMENT_NONE, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_ORNAMENT_SMALL, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_ORNAMENT_MEDIUM, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_ORNAMENT_LARGE, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_WINDOW_CLOSE_BUTTON, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_ARROW, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_ARROW_LEFT, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_ARROW_RIGHT, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_EXIT_ARROW, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_CHECKBOX, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_SLIDER, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_KNOB, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_SLOT_INVENTORY, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_SLOT_INVENTORY_SELECTED, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_SLOT_MODIFIER, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_SLOT_MODIFIER_SELECTED, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_SLOT_SPELL, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_SLOT_SPELL_SELECTED, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_SLOT_SPELL_GEM, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_SLOT_HIGHLIGHT, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_SLOT_PROGRESS_LOG, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_TAB_INACTIVE, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_TAB_ACTIVE, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_CURSOR, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_PROGRESSLOG_QUEST, ResourceType::Global);
	loadTexture(GlobalResource::TEX_GUI_PROGRESSLOG_REPUTATION, ResourceType::Global);

	loadSoundbuffer(GlobalResource::SOUND_GUI_MENUCURSOR, ResourceType::Global);
	loadSoundbuffer(GlobalResource::SOUND_GUI_PICKUP, ResourceType::Global);
	loadSoundbuffer(GlobalResource::SOUND_GUI_OPENWINDOW, ResourceType::Global);

	ConfigurationReader reader;
	if (!reader.readConfiguration(m_configuration)) {
		m_configuration = DEFAULT_CONFIGURATION;
	}
}

template<typename T> void ResourceManager::loadResource(std::map<std::string, T*>& holder, const std::string& typeName, const std::string& filename, ResourceType type, void* owner) {
	if (filename.empty()) return;
	if (holder.find(filename) != holder.end()) return; // resource already loaded

	if (type == ResourceType::Unique && owner == nullptr) {
		g_logger->logError("ResourceManager", typeName + " could not be registered as unique, owner not set: " + getPath(std::string(filename)));
		return;
	}

	T* resource = new T();

	// search project's main directory
	if (resource->loadFromFile(getPath(filename))) {
		holder[filename] = resource;
		g_logger->logInfo("ResourceManager", getPath(std::string(filename)) + ": loading " + typeName);

		switch (type) {
		case ResourceType::Unique:
			m_resourceOwners[owner].push_back(filename);
			break;

		case ResourceType::Map:
			m_mapResources.push_back(filename);
			break;

		case ResourceType::Level:
			m_levelResources.push_back(filename);
			break;

		default:
			break;
		}
	}
	else {
		g_logger->logError("ResourceManager", typeName + " could not be loaded from file: " + getPath(std::string(filename)));
		std::string tmp = typeName + " could not be loaded from file: " + getPath(filename);
		setError(ErrorID::Error_fileNotFound, tmp);
	}
}

void ResourceManager::loadTexture(const std::string& filename, ResourceType type, void* owner) {
	loadResource<sf::Texture>(m_textures, "texture", filename, type, owner);
}

void ResourceManager::loadSoundbuffer(const std::string& filename, ResourceType type, void* owner) {
	loadResource<sf::SoundBuffer>(m_soundBuffers, "sound buffer", filename, type, owner);
}

void ResourceManager::loadFont(const std::string& filename, ResourceType type, void* owner) {
	loadResource<sf::Font>(m_fonts, "font", filename, type, owner);
}

void ResourceManager::loadBitmapFont(const std::string& filename, ResourceType type, void* owner) {
	loadResource<BitmapFont>(m_bitmapFonts, "bitmap font", filename, type, owner);
}

sf::Texture* ResourceManager::getTexture(const std::string& filename) {
	const auto& it = m_textures.find(filename);
	if (it == m_textures.end()) return nullptr;
	return it->second;
}

sf::SoundBuffer* ResourceManager::getSoundBuffer(const std::string& filename) {
	const auto& it = m_soundBuffers.find(filename);
	if (it == m_soundBuffers.end()) return nullptr;
	return it->second;
}

sf::Font* ResourceManager::getFont(const std::string& filename) {
	const auto& it = m_fonts.find(filename);
	if (it == m_fonts.end()) return nullptr;
	return it->second;
}

BitmapFont* ResourceManager::getBitmapFont(const std::string& filename) {
	const auto& it = m_bitmapFonts.find(filename);
	if (it == m_bitmapFonts.end()) return nullptr;
	return it->second;
}

void ResourceManager::deleteUniqueResources(void* owner) {
	const auto &it = m_resourceOwners.find(owner);
	if (it == m_resourceOwners.end()) return;

	for (auto& resource : it->second) {
		deleteResource(resource);
	}

	it->second.clear();
	m_resourceOwners.erase(it);
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

void ResourceManager::playSound(sf::Sound& sound, const std::string& filename, bool force, float scale) {
	if (!m_configuration.isSoundOn || filename.empty()) return;
	if (m_soundBuffers.find(filename) == m_soundBuffers.end()) {
		g_logger->logError("ResourceManager", "Cannot play sound: '" + filename + "', sound not loaded!");
		return;
	}
	// don't play the sound if it's already playing and we're not forcing
	if (!force && sound.getStatus() == sf::SoundSource::Status::Playing) return;

	sound.setBuffer(*getSoundBuffer(filename));
	scale = clamp(scale, 0.f, 1.f);
	sound.setVolume(static_cast<float>(m_configuration.volumeSound) * scale);
	sound.play();
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

void ResourceManager::setError(ErrorID id, const string& description) {
	m_currentError.first = id;
	m_currentError.second = description;
}

ConfigurationData& ResourceManager::getConfiguration() {
	return m_configuration;
}

void ResourceManager::deleteLevelResources() {
	for (auto& filename : m_levelResources) {
		deleteResource(filename);
	}
	m_levelResources.clear();
}

void ResourceManager::loadMapResources() {
	loadTexture(GlobalResource::TEX_MAPMARKERS, ResourceType::Map);
	loadTexture(GlobalResource::TEX_DIALOGUE, ResourceType::Map);
	loadTexture(GlobalResource::TEX_COOKING, ResourceType::Map);
	loadSoundbuffer(GlobalResource::SOUND_TELEPORT, ResourceType::Map);
}

void ResourceManager::deleteMapResources() {
	for (auto& filename : m_mapResources) {
		deleteResource(filename);
	}
	m_mapResources.clear();
}

void ResourceManager::loadLevelResources() {
	// load level resources
	loadTexture(GlobalResource::TEX_DAMAGETYPES, ResourceType::Level);
	loadTexture(GlobalResource::TEX_DEBUFF_FEAR, ResourceType::Level);
	loadTexture(GlobalResource::TEX_DEBUFF_STUN, ResourceType::Level);
	loadTexture(GlobalResource::TEX_SCREEN_GAMEOVER, ResourceType::Level);
	loadTexture(GlobalResource::TEX_SCREEN_OVERLAY, ResourceType::Level);
	loadTexture(GlobalResource::TEX_SCREEN_OVERLAY_STUNNED, ResourceType::Level);
	loadTexture(GlobalResource::TEX_SCREEN_OVERLAY_FEARED, ResourceType::Level);
	loadTexture(GlobalResource::TEX_SCREEN_MENU, ResourceType::Level);
	loadTexture(GlobalResource::TEX_TEXT_GAMEOVER, ResourceType::Level);
	loadTexture(GlobalResource::TEX_TEXT_GAMEPAUSED, ResourceType::Level);
	loadTexture(GlobalResource::TEX_TEXT_DEFEATED, ResourceType::Level);
	// load level gui resources
	loadTexture(GlobalResource::TEX_GUI_SPEECHBUBBLE_POINTER, ResourceType::Level);
	loadTexture(GlobalResource::TEX_GUI_HEALTHBAR_MAINCHAR_BORDER, ResourceType::Level);
	loadTexture(GlobalResource::TEX_GUI_HEALTHBAR_ENEMY_BORDER, ResourceType::Level);
	loadTexture(GlobalResource::TEX_GUI_HEALTHBAR_BOSS_BORDER, ResourceType::Level);
	loadTexture(GlobalResource::TEX_GUI_HEALTHBAR_CONTENT, ResourceType::Level);
	loadTexture(GlobalResource::TEX_GUI_HEALTHBAR_CONTENT_HIT, ResourceType::Level);
	loadTexture(GlobalResource::TEX_GUI_HEALTHBAR_CONTENT_HIGHLIGHT, ResourceType::Level);
}



