#include "Map/DynamicTiles/WaypointTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"
#include "Registrar.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/TooltipComponent.h"

REGISTER_MAP_DYNAMIC_TILE(MapDynamicTileID::Waypoint, WaypointTile)

WaypointTile::WaypointTile(MapScreen* mapScreen) : MapDynamicTile(mapScreen) {
	addComponent(new TooltipComponent(g_textProvider->getText("Waypoint"), this));
	// TODO remove
	m_isAlwaysUpdate = true;
}

void WaypointTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f,
		TILE_SIZE_F,
		TILE_SIZE_F));
}

void WaypointTile::loadAnimation(int skinNr) {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	Animation* activeAnimation = new Animation();
	activeAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 5; ++i) {
		activeAnimation->addFrame(sf::IntRect(TILE_SIZE * i, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));
	}
	for (int i = 4; i > -1; --i) {
		activeAnimation->addFrame(sf::IntRect(TILE_SIZE * i, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));
	}

	addAnimation(GameObjectState::Active, activeAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(250, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

void WaypointTile::update(const sf::Time& frameTime) {
	MapDynamicTile::update(frameTime);
	if (m_state == GameObjectState::Active) return;
	//if (m_mainChar->getBoundingBox()->intersects(*getBoundingBox())) {
	activateWaypoint();
}

void WaypointTile::activateWaypoint() {
	m_screen->getCharacterCore()->setWaypointUnlocked(m_map->getID(), m_spawnPosition, m_portPosition);
	dynamic_cast<MapScreen*>(m_screen)->notifyWaypointUnlocked();
	m_screen->setTooltipText("WaypointActivated", COLOR_GOOD, true);
	g_resourceManager->playSound(getSoundPath());
	setActive();
}

void WaypointTile::onLeftClick() {
	onRightClick();
}

void WaypointTile::onRightClick() {
	if (m_state == GameObjectState::Idle) {
		m_screen->setNegativeTooltip("WaypointNotActive");
		return;
	}
	
	m_mainChar->setPosition(m_portPosition);
	g_resourceManager->playSound(getSoundPath());
}

void WaypointTile::setPosition(const sf::Vector2f& pos) {
	MapDynamicTile::setPosition(pos);

	// calculate port pos.
	const sf::FloatRect& bb = *m_mainChar->getBoundingBox();
	m_portPosition = sf::Vector2f(
		getPosition().x + TILE_SIZE_F * 0.5f - bb.width * 0.5f,
		getPosition().y - bb.height + TILE_SIZE_F * 0.5f
	);
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

std::string WaypointTile::getSpritePath() const {
	return "res/assets/map_dynamic_tiles/spritesheet_tiles_waypoint.png";
}

std::string WaypointTile::getSoundPath() const {
	return GlobalResource::SOUND_TELEPORT;
}

