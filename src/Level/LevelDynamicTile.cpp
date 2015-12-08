#include "Level/LevelDynamicTile.h"

LevelDynamicTile::LevelDynamicTile(Level* level) : AnimatedGameObject() {
	m_level = level;
}

bool LevelDynamicTile::getIsCollidable() const {
	return m_isCollidable;
}

void LevelDynamicTile::setTileSize(const sf::Vector2i& tileSize) {
	m_tileSize = tileSize;
}

void LevelDynamicTile::setPositionOffset(const sf::Vector2f& offset) {
	m_positionOffset = offset;
}

const sf::Vector2f&  LevelDynamicTile::getPositionOffset() const {
	return m_positionOffset;
}

GameObjectType LevelDynamicTile::getConfiguredType() const {
	return GameObjectType::_DynamicTile;
}

void LevelDynamicTile::onHit(LevelMovableGameObject* mob) {
	// nop
}
