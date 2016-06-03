#include "GUI/MapOverlay.h"
#include "GUI/Window.h"
#include "Screens/MapScreen.h"
#include "Map/DynamicTiles/WaypointTile.h"
#include "Map/MapMainCharacter.h"
#include "GlobalResource.h"

float MapOverlay::TOP = 30.f;
float MapOverlay::LEFT = 30.f;

MapOverlay::MapOverlay(MapScreen* screen) :
	// copy those maps
	m_backgroundTileMap(*screen->getWorld()->getBackgroundTileMap()),
	m_lightedForegroundTileMap(*screen->getWorld()->getLightedForegroundTileMap()),
	m_foregroundTileMap(*screen->getWorld()->getForegroundTileMap()) {

	m_screen = screen;

	const Map& map = *m_screen->getWorld();

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

	if (map.getWorldData()->explorable) {
		m_fogOfWarTileMap.initFogOfWar(*map.getWorldData(), m_screen->getCharacterCore());
		m_fogOfWarTileMap.setScale(sf::Vector2f(m_scale, m_scale));
		m_fogOfWarTileMap.setPosition(m_position);
		m_explorable = true;
	}

	m_mainCharMarker.setTexture(*g_resourceManager->getTexture(GlobalResource::TEX_MAPMARKERS));
	m_mainCharMarker.setTextureRect(sf::IntRect(0, 0, 25, 25));

	m_title.setString(g_textProvider->getText(map.getName(), "location"));
	m_title.setCharacterSize(16);
	m_title.setPosition(sf::Vector2f((WINDOW_WIDTH - m_title.getBounds().width) / 2.f, m_boundingBox.top - 24.f));

	sf::FloatRect box(m_position.x - 1.f, m_position.y - 1.f, m_boundingBox.width + 2.f, m_boundingBox.height + 2.f);
	m_border = new Window(box,
		GUIOrnamentStyle::LARGE,
		COLOR_TRANSPARENT,
		GUIConstants::ORNAMENT_COLOR);

	m_border->addCloseButton(std::bind(&MapOverlay::hide, this));
}

MapOverlay::~MapOverlay() {
	for (auto& wp : m_waypoints) {
		delete wp;
	}
	m_waypoints.clear();
	delete m_border;
}

void MapOverlay::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;

	if (m_explorable) {
		const Map& map = *m_screen->getWorld();
		m_fogOfWarTileMap.updateFogOfWar(*map.getWorldData(), m_screen->getCharacterCore());
	}

	m_border->update(frameTime);

	m_mainCharMarker.setPosition(m_position + 
		m_screen->getMainCharacter()->getCenter() * m_scale - sf::Vector2f(12.5f, 12.5f));

	for (auto& wp : m_waypoints) {
		wp->update(frameTime);
	}

	if (g_inputController->isMouseOver(&m_boundingBox, true)) {
		g_inputController->lockAction();
	}
} 

void MapOverlay::reloadWaypoints() {
	for (auto& wp : m_waypoints) {
		delete wp;
	}
	m_waypoints.clear();

	const std::vector<GameObject*>* tiles = m_screen->getObjects(GameObjectType::_DynamicTile);
	for (auto& go : *tiles) {
		if (WaypointTile* tile = dynamic_cast<WaypointTile*>(go)) {
			if (tile->getGameObjectState() == GameObjectState::Idle) continue;

			WaypointMarker* marker = new WaypointMarker(m_screen->getMainCharacter(), tile->getPosition(), this);
			marker->loadAnimation();
			marker->setPosition(m_position + tile->getCenter() * m_scale -
				sf::Vector2f(12.5f, 12.5f));
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

	if (m_explorable) {
		target.draw(m_fogOfWarTileMap);
	}

	for (auto& wp : m_waypoints) {
		wp->render(target);
	}

	target.draw(m_mainCharMarker);
	target.draw(m_title);

	m_border->render(target);
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

	m_tooltip.setCharacterSize(8);
	m_tooltip.setString(g_textProvider->getText("ClickToTeleport"));
}

void WaypointMarker::setPosition(const sf::Vector2f& position) {
	AnimatedGameObject::setPosition(position);
	m_tooltip.setPosition(position + sf::Vector2f(0.f, -8.f));
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
	const sf::FloatRect& bb = *m_mainChar->getBoundingBox();
	m_mainChar->setPosition(sf::Vector2f(
		m_waypointPosition.x + TILE_SIZE_F / 2.f - bb.width / 2.f,
		m_waypointPosition.y - bb.height + TILE_SIZE_F / 2.f
		));
	g_resourceManager->playSound(m_sound, GlobalResource::SOUND_TELEPORT, true);
	m_parent->hide();
}

void WaypointMarker::onLeftClick() {
	onRightClick();
}

GameObjectType WaypointMarker::getConfiguredType() const {
	return GameObjectType::_Interface;
}


