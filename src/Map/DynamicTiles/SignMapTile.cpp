#include "Map/DynamicTiles/SignMapTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"

SignMapTile::SignMapTile(const SignData& data, MapScreen* mapScreen) : MapDynamicTile(mapScreen) {
	m_data = data;
}

void SignMapTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, 20.f));
	setPositionOffset(sf::Vector2f(0.f, 15.f));
	setSpriteOffset(sf::Vector2f(0.f, -15.f));
}

void SignMapTile::loadAnimation(int skinNr) {
	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_sign));
	idleAnimation->addFrame(sf::IntRect(0, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(false);
}

void SignMapTile::onMouseOver() {
	setSpriteColor(COLOR_INTERACTIVE, sf::milliseconds(100));
}

GameObjectType SignMapTile::getConfiguredType() const {
	return GameObjectType::_ForegroundDynamicTile;
}

