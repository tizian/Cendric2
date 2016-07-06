#include "Map/MapDynamicTile.h"
#include "Screens/MapScreen.h"

MapDynamicTile::MapDynamicTile(MapScreen* mapScreen) : AnimatedGameObject() {
	m_map = mapScreen->getWorld();
	m_screen = mapScreen;
	m_mainChar = mapScreen->getMainCharacter();
}

bool MapDynamicTile::getIsCollidable() const {
	return m_isCollidable;
}

void MapDynamicTile::setPositionOffset(const sf::Vector2f& offset) {
	m_positionOffset = offset;
}

const sf::Vector2f&  MapDynamicTile::getPositionOffset() const {
	return m_positionOffset;
}

GameObjectType MapDynamicTile::getConfiguredType() const {
	return GameObjectType::_DynamicTile;
}

void MapDynamicTile::onHit(MapMainCharacter* mob) {
	// nop
}

void MapDynamicTile::loadResources() {
	g_resourceManager->loadTexture(getSpritePath(), ResourceType::Map);
	g_resourceManager->loadSoundbuffer(getSoundPath(), ResourceType::Map);
}
