#include "GUI/MapOverlay.h"
#include "Screens/MapScreen.h"
#include "Map/DynamicTiles/WaypointTile.h"
#include "Map/MapMainCharacter.h"

float MapOverlay::TOP = 20.f;
float MapOverlay::LEFT = 20.f;

MapOverlay::MapOverlay(MapScreen* screen) :
	// copy those maps
	m_backgroundTileMap(*screen->getMap().getBackgroundTileMap()),
	m_lightedForegroundTileMap(*screen->getMap().getLightedForegroundTileMap()),
	m_foregroundTileMap(*screen->getMap().getForegroundTileMap()) {

	m_screen = screen;

	const Map& map = m_screen->getMap();

	sf::Vector2f mapSize(map.getWorldRect().width, map.getWorldRect().height);
	// check out the limiting factor for our scale
	if (mapSize.x / WINDOW_WIDTH > mapSize.y / WINDOW_HEIGHT) {
		m_boundingBox.width = WINDOW_WIDTH - 2 * LEFT;
		m_scale = m_boundingBox.width / mapSize.x;
		m_boundingBox.height = m_scale * mapSize.y;
		m_boundingBox.left = LEFT;
		m_boundingBox.top = (WINDOW_HEIGHT - m_boundingBox.height) / 2.f;
	}
	else {
		m_boundingBox.height = WINDOW_HEIGHT - 2 * TOP;
		m_scale = m_boundingBox.height / mapSize.y;
		m_boundingBox.width = m_scale * mapSize.x;
		m_boundingBox.top = TOP;
		m_boundingBox.left = (WINDOW_WIDTH - m_boundingBox.width) / 2.f;
	}

	m_position.x = m_boundingBox.left;
	m_position.y = m_boundingBox.top;

	m_backgroundTileMap.setScale(sf::Vector2f(m_scale, m_scale));
	m_lightedForegroundTileMap.setScale(sf::Vector2f(m_scale, m_scale));
	m_foregroundTileMap.setScale(sf::Vector2f(m_scale, m_scale));

	m_backgroundTileMap.setPosition(m_position);
	m_lightedForegroundTileMap.setPosition(m_position);
	m_foregroundTileMap.setPosition(m_position);

	m_mainCharMarker.setTexture(*g_resourceManager->getTexture(ResourceID::Texture_mapmarkers));
	m_mainCharMarker.setTextureRect(sf::IntRect(0, 0, 25, 25));
}

MapOverlay::~MapOverlay() {
	
}

void MapOverlay::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;

	m_mainCharMarker.setPosition(m_position + 
		m_screen->getMainCharacter()->getCenter() * m_scale - sf::Vector2f(12.5f, 12.5f));

	for (auto& wp : m_waypoints) {
		wp.update(frameTime);
	}

	if (g_inputController->isMouseOver(&m_boundingBox, true)) {
		g_inputController->lockAction();
	}
} 

void MapOverlay::reloadWaypoints() {
	m_waypoints.clear();

	const std::vector<GameObject*>* tiles = m_screen->getObjects(GameObjectType::_DynamicTile);
	for (auto& go : *tiles) {
		if (WaypointTile* tile = dynamic_cast<WaypointTile*>(go)) {
			if (tile->getGameObjectState() == GameObjectState::Idle) continue;

			WaypointMarker marker(m_screen->getMainCharacter(), tile->getPosition(), this);
			marker.setPosition(m_position + tile->getPosition() * m_scale +
				sf::Vector2f(TILE_SIZE * m_scale - 12.5f, TILE_SIZE * m_scale - 12.5f));
			m_waypoints.push_back(marker);
		}
	}
}

bool MapOverlay::isVisible() const {
	return m_isVisible;
}

void MapOverlay::render(sf::RenderTarget& target) {
	if (!m_isVisible) return;

	target.draw(m_backgroundTileMap);
	target.draw(m_lightedForegroundTileMap);
	target.draw(m_foregroundTileMap);

	for (auto& wp : m_waypoints) {
		wp.render(target);
	}

	target.draw(m_mainCharMarker);
}

void MapOverlay::show() {
	reloadWaypoints();
	m_isVisible = true;
}

void MapOverlay::hide() {
	m_isVisible = false;
}

/////////// WAYPOINT MARKER /////////////

WaypointMarker::WaypointMarker(MapMainCharacter* mainChar, const sf::Vector2f& waypointPosition, MapOverlay* parent) {
	m_parent = parent;
	m_mainChar = mainChar;
	m_waypointPosition = waypointPosition;
	m_isInputInDefaultView = true;

	// load animations
	setBoundingBox(sf::FloatRect(0.f, 0.f, 25.f, 25.f));

	Animation activeAnimation;
	activeAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_mapmarkers));
	activeAnimation.addFrame(sf::IntRect(50, 0, 25, 25));

	addAnimation(GameObjectState::Active, activeAnimation);

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_mapmarkers));
	idleAnimation.addFrame(sf::IntRect(25, 0, 25, 25));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(false);
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

void WaypointMarker::onRightClick() {
	const sf::FloatRect& bb = *m_mainChar->getBoundingBox();
	m_mainChar->setPosition(sf::Vector2f(
		m_waypointPosition.x + TILE_SIZE_F / 2.f - bb.width / 2.f,
		m_waypointPosition.y - bb.height + TILE_SIZE_F / 2.f
		));
	m_parent->hide();
}

void WaypointMarker::onLeftClick() {
	onRightClick();
}

void WaypointMarker::onInteractKey() {
	onRightClick();
}

GameObjectType WaypointMarker::getConfiguredType() const {
	return GameObjectType::_Interface;
}


