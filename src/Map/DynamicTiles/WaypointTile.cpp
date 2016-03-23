#include "Map/DynamicTiles/WaypointTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"
#include "Registrar.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/TooltipComponent.h"

REGISTER_MAP_DYNAMIC_TILE(MapDynamicTileID::Waypoint, WaypointTile)

WaypointTile::WaypointTile(MapScreen* mapScreen) : MapDynamicTile(mapScreen) {
	addComponent(new TooltipComponent(g_textProvider->getText("Fireplace"), this));
}

void WaypointTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f,
		TILE_SIZE_F,
		TILE_SIZE_F));
}

void WaypointTile::loadAnimation(int skinNr) {

	Animation* activeAnimation = new Animation();
	activeAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_waypoint));
	for (int i = 0; i < 5; ++i) {
		activeAnimation->addFrame(sf::IntRect(TILE_SIZE * i, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));
	}
	for (int i = 4; i > -1; --i) {
		activeAnimation->addFrame(sf::IntRect(TILE_SIZE * i, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));
	}

	addAnimation(GameObjectState::Active, activeAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_waypoint));
	idleAnimation->addFrame(sf::IntRect(250, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

void WaypointTile::update(const sf::Time& frameTime) {
	MapDynamicTile::update(frameTime);
	if (m_state == GameObjectState::Active) return;
	if (m_mainChar->getBoundingBox()->intersects(*getBoundingBox())) {
		m_screen->getCharacterCore()->setWaypointUnlocked(m_map->getID(), m_spawnPosition);
		m_screen->setTooltipText("WaypointActivated", COLOR_GOOD, true);
		setActive();
	}
}

void WaypointTile::onRightClick() {
	if (m_state == GameObjectState::Idle) {
		m_screen->setTooltipText("WaypointNotActive", COLOR_BAD, true);
		return;
	}
	const sf::FloatRect& bb = *m_mainChar->getBoundingBox();
	m_mainChar->setPosition(sf::Vector2f(
		getPosition().x + TILE_SIZE_F * 0.5f - bb.width * 0.5f,
		getPosition().y - bb.height + TILE_SIZE_F * 0.5f
		));
}

void WaypointTile::setActive() {
	setState(GameObjectState::Active);

	addComponent(new LightComponent(LightData(
		sf::Vector2f(TILE_SIZE_F * 0.5f, TILE_SIZE_F * 0.5f),
		sf::Vector2f(100.f, 100.f)), this));
}

void WaypointTile::setSpawnPosition(int spawnPosition) {
	m_spawnPosition = spawnPosition;
}

