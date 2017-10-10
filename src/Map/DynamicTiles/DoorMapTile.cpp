#include "Map/DynamicTiles/DoorMapTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"
#include "GameObjectComponents/InteractComponent.h"
#include "Registrar.h"

REGISTER_MAP_DYNAMIC_TILE(MapDynamicTileID::Door, DoorMapTile)

DoorMapTile::DoorMapTile(MapScreen* mapScreen) : MapDynamicTile(mapScreen) {
	m_interactComponent = new InteractComponent(g_textProvider->getText("Door"), this, m_mainChar);
	m_interactComponent->setInteractRange(getOpenRange());
	m_interactComponent->setInteractText("ToOpen");
	m_interactComponent->setOnInteract(std::bind(&DoorMapTile::onRightClick, this));
	addComponent(m_interactComponent);
}

void DoorMapTile::open() {
	m_isOpen = true;
	m_isCollidable = false;
	setState(GameObjectState::Open);
	m_interactComponent->setInteractable(false);
}

void DoorMapTile::close() {
	m_isOpen = false;
	m_isCollidable = true;
	setState(GameObjectState::Closed);
	m_interactComponent->setInteractable(true);
}

void DoorMapTile::update(const sf::Time& frameTime) {
	if (m_isReloadNeeded) {
		reloadConditions(m_mainChar);
	}

	MapDynamicTile::update(frameTime);
}

bool DoorMapTile::init(const MapTileProperties& properties) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
	setPositionOffset(sf::Vector2f(0.f, 0.f));
	setSpriteOffset(sf::Vector2f(0.f, 0.f));

	initConditions(properties);

	return true;
}

void DoorMapTile::loadAnimation(int skinNr) {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	Animation* openAnimation = new Animation();
	openAnimation->setSpriteSheet(tex);
	openAnimation->addFrame(sf::IntRect(0, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Open, openAnimation);

	Animation* closedAnimation = new Animation();
	closedAnimation->setSpriteSheet(tex);
	closedAnimation->addFrame(sf::IntRect(TILE_SIZE, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Closed, closedAnimation);

	playCurrentAnimation(false);
	reloadConditions(m_mainChar);
}

float DoorMapTile::getOpenRange() const {
	return 50.f;
}

std::string DoorMapTile::getSpritePath() const {
	return "res/texture/map_dynamic_tiles/spritesheet_tiles_door.png";
}

void DoorMapTile::onRightClick() {
	if (m_isOpen) return;

	// check if the door is in range
	bool inRange = dist(m_mainChar->getCenter(), getCenter()) <= getOpenRange();

	if (!inRange) {
		m_screen->setNegativeTooltip("OutOfRange");
	}
	else if (m_isConditionsFulfilled && !m_keyItemID.empty() && m_screen->getCharacterCore()->hasItem(m_keyItemID, 1)) {
		open();
		g_resourceManager->playSound(GlobalResource::SOUND_MISC_UNLOCK);

		std::string tooltipText = g_textProvider->getText("Used");
		tooltipText.append(g_textProvider->getText(m_keyItemID, "item"));
		m_screen->setTooltipTextRaw(tooltipText, COLOR_GOOD, true);

		g_inputController->lockAction();
	}
	else if (!m_keyItemID.empty()) {
		m_screen->setNegativeTooltip("IsLockedKey");
	}
	else {
		m_screen->setNegativeTooltip("IsLocked");
	}
}
