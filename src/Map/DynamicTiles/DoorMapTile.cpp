#include "Map/DynamicTiles/DoorMapTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"
#include "GameObjectComponents/InteractComponent.h"

const float DoorMapTile::TOOLTIP_TOP = 20.f;
const float DoorMapTile::OPEN_RANGE = 50.f;

DoorMapTile::DoorMapTile(const DoorData& data, MapScreen* mapScreen) : MapDynamicTile(mapScreen) {
	m_data = data;

	m_interactComponent = new InteractComponent(g_textProvider->getText("Door"), this, m_mainChar);
	m_interactComponent->setInteractRange(OPEN_RANGE);
	m_interactComponent->setInteractText("ToOpen");
	m_interactComponent->setOnInteract(std::bind(&DoorMapTile::onInteract, this));
	addComponent(m_interactComponent);

	m_isCollidable = true;
}

void DoorMapTile::onInteract() {

}

void DoorMapTile::update(const sf::Time& frameTime) {
	m_interactComponent->setInteractable(true);
	MapDynamicTile::update(frameTime);
}

void DoorMapTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
	setPositionOffset(sf::Vector2f(0.f, 0.f));
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
}

void DoorMapTile::loadAnimation(int skinNr) {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(false);
}

std::string DoorMapTile::getSpritePath() const {
	return "res/assets/map_dynamic_tiles/spritesheet_tiles_doors.png";
}

void DoorMapTile::setPosition(const sf::Vector2f& pos) {
	MapDynamicTile::setPosition(pos);
}

void DoorMapTile::onMouseOver() {
	setSpriteColor(COLOR_INTERACTIVE, sf::milliseconds(100));
	m_interactComponent->setInteractable(false);
}

void DoorMapTile::renderAfterForeground(sf::RenderTarget& renderTarget) {
	MapDynamicTile::renderAfterForeground(renderTarget);
}

GameObjectType DoorMapTile::getConfiguredType() const {
	return GameObjectType::_ForegroundDynamicTile;
}