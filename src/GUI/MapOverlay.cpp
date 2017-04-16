#include "GUI/MapOverlay.h"
#include "GUI/Window.h"
#include "GUI/GUITabBar.h"
#include "Screens/WorldScreen.h"
#include "Map/DynamicTiles/WaypointTile.h"
#include "MainCharacter.h"
#include "GlobalResource.h"
#include "GUI/GUIConstants.h"
#include "Structs/LevelData.h"

const float MapOverlay::TOP = 30.f;
const float MapOverlay::LEFT = GUIConstants::LEFT;
const float MapOverlay::MAX_WIDTH = WINDOW_WIDTH - 2 * LEFT;
const float MapOverlay::MAX_HEIGHT = WINDOW_HEIGHT - 2 * TOP;

MapOverlay::MapOverlay(WorldScreen* screen, GUITabBar* mapTabBar) {
	m_screen = screen;
	m_mapTabBar = mapTabBar;

	const World& map = *m_screen->getWorld();

	m_mainCharMarker.setTexture(*g_resourceManager->getTexture(GlobalResource::TEX_MAPMARKERS));
	m_mainCharMarker.setTextureRect(sf::IntRect(0, 0, 25, 25));

	m_title.setCharacterSize(GUIConstants::CHARACTER_SIZE_L);

	m_window = new Window(sf::FloatRect(),
		GUIOrnamentStyle::LARGE,
		COLOR_TRANSPARENT,
		GUIConstants::ORNAMENT_COLOR);

	m_window->addCloseButton(std::bind(&MapOverlay::hide, this));

	reloadMaps();

	const std::string mapName = m_screen->getCharacterCore()->getData().currentMap;

	setMap(mapName);
	if (m_currentMap == -1) {
		setMapIndex(0);
	}
}

MapOverlay::~MapOverlay() {
	for (auto& wp : m_waypoints) {
		delete wp;
	}
	m_waypoints.clear();

	for (auto& m : m_maps) {
		delete m;
	}
	m_maps.clear();

	delete m_window;
}

void MapOverlay::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;
	auto* map = getCurrentMap();
	if (map == nullptr) return;

	if (m_isOnCurrentMap) {
		map->fogOfWarTileMap.updateFogOfWar(m_screen->getCharacterCore()->getExploredTiles().at(map->mapId).second);
		m_mainCharMarker.setPosition(m_position +
			m_screen->getMainCharacter()->getCenter() * map->scale - sf::Vector2f(12.5f, 12.5f));
	}

	for (auto& wp : m_waypoints) {
		wp->update(frameTime);
	}

	m_window->update(frameTime);
}


void MapOverlay::setMap(const std::string& mapID) {
	int index = 0;

	for (auto& it : m_maps) {
		if (it->mapId.compare(mapID) == 0) {
			setMapIndex(index);
			break;
		}
		index++;
	}
}

void MapOverlay::setMapIndex(int index) {
	if (index < 0 || index > static_cast<int>(m_maps.size()) - 1) return;

	auto const& map = m_maps[index];

	m_boundingBox = map->windowSize;
	m_position.x = m_boundingBox.left;
	m_position.y = m_boundingBox.top;

	m_window->setSize(sf::Vector2f(m_boundingBox.width + 1.f, m_boundingBox.height + 1.f));
	m_window->setPosition(sf::Vector2f(m_position.x - 1.f, m_position.y - 1.f));

	map->map.setPosition(m_position);
	map->fogOfWarTileMap.setPosition(m_position);
	map->fogOfWarTileMap.updateFogOfWar(m_screen->getCharacterCore()->getExploredTiles().at(map->mapId).second);

	m_title.setString(g_textProvider->getText(World::getNameFromId(map->mapId), "location"));
	m_title.setPosition(sf::Vector2f((WINDOW_WIDTH - m_title.getBounds().width) / 2.f, m_boundingBox.top - 24.f));

	m_isOnCurrentMap = (m_screen->getWorldData()->id.compare(map->mapId) == 0);
	m_currentMap = index;

	reloadWaypoints();
}

void MapOverlay::reloadMaps() {
	for (auto& m : m_maps) {
		delete m;
	}
	m_maps.clear();

	int buttonIndex = 0;

	for (auto& explored : m_screen->getCharacterCore()->getData().tilesExplored) {
		// load map texture
		const std::string mapFilename = getMapSpriteFilename(explored.first);
		const std::string iconFilename = getMapIconFilename(explored.first);

		g_resourceManager->loadTexture(mapFilename, ResourceType::Global);
		g_resourceManager->loadTexture(iconFilename, ResourceType::Global);

		if (g_resourceManager->getTexture(mapFilename) == nullptr) continue;
		if (g_resourceManager->getTexture(iconFilename) == nullptr) continue;

		MapOverlayData* data = new MapOverlayData();
		data->mapId = explored.first;
		data->map = sf::Sprite(*g_resourceManager->getTexture(mapFilename));

		data->mapSize = explored.second.first;
		sf::Vector2f mapSize = sf::Vector2f(data->mapSize.x * TILE_SIZE_F, data->mapSize.y * TILE_SIZE_F);
		sf::Vector2f spriteSize = sf::Vector2f(static_cast<float>(data->map.getTextureRect().width), static_cast<float>(data->map.getTextureRect().height));
		
		// check out the limiting factor for our scale
		if (data->mapSize.x / MAX_WIDTH > data->mapSize.y / MAX_HEIGHT) {
			data->windowSize.width = MAX_WIDTH;
			data->scale = data->windowSize.width / mapSize.x;
			data->windowSize.height = data->scale * mapSize.y;
			data->windowSize.left = LEFT;
			data->windowSize.top = (WINDOW_HEIGHT - data->windowSize.height) / 2.f;
		}
		else {
			data->windowSize.height = MAX_HEIGHT;
			data->scale = data->windowSize.height / mapSize.y;
			data->windowSize.width = data->scale * mapSize.x;
			data->windowSize.top = TOP;
			data->windowSize.left = (WINDOW_WIDTH - data->windowSize.width) / 2.f;
		}

		m_position.x = m_boundingBox.left;
		m_position.y = m_boundingBox.top;

		data->fogOfWarTileMap.initFogOfWar(data->mapSize);
		data->fogOfWarTileMap.setScale(data->scale, data->scale);
		data->map.setScale(data->windowSize.width / spriteSize.x, data->windowSize.height / spriteSize.y);

		// load buttons
		sf::Texture* tex = g_resourceManager->getTexture(iconFilename);
		m_mapTabBar->setButtonTexture(buttonIndex, tex, 0);
		m_mapTabBar->setButtonOnClick(buttonIndex, std::bind(&MapOverlay::setMap, this, data->mapId));

		m_maps.push_back(data);

		buttonIndex++;
	}
}

MapOverlayData* MapOverlay::getCurrentMap() const {
	if (m_currentMap < 0 || m_currentMap > static_cast<int>(m_maps.size())) return nullptr;
	return m_maps[m_currentMap];
}

WorldScreen* MapOverlay::getScreen() const {
	return m_screen;
}

void MapOverlay::reloadWaypoints() {
	for (auto& wp : m_waypoints) {
		delete wp;
	}
	m_waypoints.clear();

	const LevelData* lData = dynamic_cast<const LevelData*>(m_screen->getWorldData());
	if (lData && lData->isBossLevel) {
		return;
	}

	auto* map = getCurrentMap();
	if (map == nullptr) return;

	if (!contains(m_screen->getCharacterCore()->getData().waypointsUnlocked, map->mapId)) return;

	for (auto& it : m_screen->getCharacterCore()->getData().waypointsUnlocked.at(map->mapId)) {
		WaypointMarker* marker = new WaypointMarker(m_screen->getMainCharacter(), it.second, this);
		marker->loadAnimation();
		marker->setPosition(m_position + (it.second + sf::Vector2f(TILE_SIZE_F * 0.19f, TILE_SIZE_F * 0.23f)) * map->scale -
			sf::Vector2f(12.5f, 12.5f));
		m_waypoints.push_back(marker);
	}
}

bool MapOverlay::isVisible() const {
	return m_isVisible;
}

bool MapOverlay::isOnCurrentMap() const {
	return m_isOnCurrentMap;
}

void MapOverlay::render(sf::RenderTarget& target) {
	if (!m_isVisible) return;
	auto* map = getCurrentMap();
	if (map == nullptr) return;

	target.draw(map->map);
	target.draw(map->fogOfWarTileMap);

	if (m_isOnCurrentMap)
		target.draw(m_mainCharMarker);
	target.draw(m_title);

	m_window->render(target);

	for (auto& wp : m_waypoints) {
		wp->render(target);
	}
}

void MapOverlay::show() {
	reloadWaypoints();
	m_mapTabBar->show(m_currentMap);
	m_isVisible = true;
}

void MapOverlay::hide() {
	m_mapTabBar->hide();
	m_isVisible = false;
}

std::string MapOverlay::getMapSpriteFilename(const std::string& mapID) {
	std::string filename = mapID;
	if (filename.size() < 4) return "";
	return filename.substr(0, filename.size() - 4) + ".png";
}

std::string MapOverlay::getMapIconFilename(const std::string& mapID) {
	std::string filename = mapID;
	if (filename.size() < 4) return "";
	return filename.substr(0, filename.size() - 4) + "_icon.png";
}

/////////// WAYPOINT MARKER /////////////

WaypointMarker::WaypointMarker(MainCharacter* mainChar, const sf::Vector2f& waypointPosition, MapOverlay* parent) {
	m_parent = parent;
	m_mainChar = mainChar;
	m_waypointPosition = waypointPosition;
	m_isInputInDefaultView = true;
}

void WaypointMarker::loadAnimation() {
	// load animations
	setBoundingBox(sf::FloatRect(0.f, 0.f, 25.f, 25.f));

	Animation* activeAnimation = new Animation();
	activeAnimation->setSpriteSheet(g_resourceManager->getTexture(GlobalResource::TEX_MAPMARKERS));
	for (int i = 2; i < 6; i++)
		activeAnimation->addFrame(sf::IntRect(i * 25, 0, 25, 25));

	for (int i = 5; i > 1; i--)
		activeAnimation->addFrame(sf::IntRect(i * 25, 0, 25, 25));

	addAnimation(GameObjectState::Active, activeAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(GlobalResource::TEX_MAPMARKERS));
	idleAnimation->addFrame(sf::IntRect(25, 0, 25, 25));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	m_tooltip.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_tooltip.setTextStyle(TextStyle::Shadowed);
	m_tooltip.setString(g_textProvider->getText("ClickToTeleport"));
}

void WaypointMarker::setPosition(const sf::Vector2f& position) {
	AnimatedGameObject::setPosition(position);
	m_tooltip.setPosition(position + sf::Vector2f(-0.5f * (m_tooltip.getBounds().width - m_boundingBox.width), -8.f));
}

void WaypointMarker::onMouseOver() {
	m_isMouseOver = true;
	setState(GameObjectState::Active);
}

void WaypointMarker::update(const sf::Time& frameTime) {
	bool wasMouseOver = m_isMouseOver;
	m_isMouseOver = false;
	AnimatedGameObject::update(frameTime);
	if (wasMouseOver && !m_isMouseOver) {
		setState(GameObjectState::Idle);
	}
}

void WaypointMarker::render(sf::RenderTarget& target) {
	AnimatedGameObject::render(target);
	if (m_state == GameObjectState::Active) {
		target.draw(m_tooltip);
	}
}

void WaypointMarker::onRightClick() {
	if (m_parent->isOnCurrentMap()) {
		m_mainChar->setPosition(m_waypointPosition);
	}
	else {
		TriggerContent tc;
		tc.type = TriggerContentType::MapEntry;
		tc.s1 = m_parent->getCurrentMap()->mapId;
		tc.i1 = static_cast<int>(m_waypointPosition.x);
		tc.i2 = static_cast<int>(m_waypointPosition.y);

		TriggerContent::executeTrigger(tc, m_parent->getScreen());
	}
	
	g_resourceManager->playSound(GlobalResource::SOUND_TELEPORT);
	m_parent->hide();
}

void WaypointMarker::onLeftClick() {
	onRightClick();
}

GameObjectType WaypointMarker::getConfiguredType() const {
	return GameObjectType::_Interface;
}


