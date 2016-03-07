#include "Map/DynamicTiles/WaypointTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"
#include "Registrar.h"

REGISTER_MAP_DYNAMIC_TILE(MapDynamicTileID::Waypoint, WaypointTile)

WaypointTile::WaypointTile(Map* map) : MapDynamicTile(map) {
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

	// init tooltip
	m_tooltipText.setTextStyle(TextStyle::Shadowed);
	m_tooltipText.setString(g_textProvider->getText("Waypoint"));
	m_tooltipText.setColor(sf::Color::White);
	m_tooltipText.setCharacterSize(8);
}

void WaypointTile::update(const sf::Time& frameTime) {
	MapDynamicTile::update(frameTime);
	GameObject::updateTime(m_tooltipTime, frameTime);
	if (m_state == GameObjectState::Active) return;
	if (m_mainCharacter->getBoundingBox()->intersects(*getBoundingBox())) {
		m_screen->getCharacterCore()->setWaypointUnlocked(m_map->getID(), m_spawnPosition);
		m_screen->setTooltipText("WaypointActivated", sf::Color::Green, true);
		setActive();
	}
}

void WaypointTile::renderAfterForeground(sf::RenderTarget &renderTarget) {
	MapDynamicTile::renderAfterForeground(renderTarget);
	bool showTooltip = g_inputController->isKeyActive(Key::ToggleTooltips);
	if (showTooltip || m_tooltipTime > sf::Time::Zero) {
		renderTarget.draw(m_tooltipText);
	}
}

void WaypointTile::onMouseOver() {
	m_tooltipTime = sf::seconds(1);
}

void WaypointTile::onRightClick() {
	if (m_state == GameObjectState::Idle) {
		m_screen->setTooltipText("WaypointNotActive", sf::Color::Red, true);
		return;
	}
	const sf::FloatRect& bb = *m_mainCharacter->getBoundingBox();
	m_mainCharacter->setPosition(sf::Vector2f(
		getPosition().x + TILE_SIZE_F / 2.f - bb.width / 2.f,
		getPosition().y - bb.height + TILE_SIZE_F / 2.f
		));
}

void WaypointTile::setPosition(const sf::Vector2f& pos) {
	if (m_lightObject != nullptr) m_lightObject->setPosition(pos + sf::Vector2f(getBoundingBox()->width / 2.f, getBoundingBox()->height / 2.f));
	MapDynamicTile::setPosition(pos);
	m_tooltipText.setPosition(sf::Vector2f(getPosition().x, getPosition().y - 10.f));
}

void WaypointTile::setActive() {
	setState(GameObjectState::Active);
	if (m_lightObject != nullptr) m_lightObject->setDisposed();
	m_lightObject = new LightObject(LightData(sf::Vector2f(), sf::Vector2f(70.f, 50.f), 0.5f));
	m_lightObject->setPosition(getPosition() + sf::Vector2f(getBoundingBox()->width / 2.f, getBoundingBox()->height / 2.f));
	m_screen->addObject(m_lightObject);
}

void WaypointTile::setSpawnPosition(int spawnPosition) {
	m_spawnPosition = spawnPosition;
}

void WaypointTile::setDisposed() {
	MapDynamicTile::setDisposed();
	if (m_lightObject != nullptr) m_lightObject->setDisposed();
}

void WaypointTile::setScreen(Screen* screen) {
	MapDynamicTile::setScreen(screen);
	m_mainCharacter = dynamic_cast<MapScreen*>(screen)->getMainCharacter();
}
