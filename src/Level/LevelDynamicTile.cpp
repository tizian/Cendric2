#include "Level/LevelDynamicTile.h"
#include "Screens/LevelScreen.h"
#include "Level/LevelMainCharacter.h"

LevelDynamicTile::LevelDynamicTile(LevelScreen* levelScreen) : AnimatedGameObject() {
	m_level = levelScreen->getWorld();
	m_screen = levelScreen;
	m_mainChar = levelScreen->getMainCharacter();
}

bool LevelDynamicTile::getIsCollidable() const {
	return m_isCollidable;
}

bool LevelDynamicTile::getIsStrictlyCollidable() const {
	return m_isStrictlyCollidable;
}

void LevelDynamicTile::setPositionOffset(const sf::Vector2f& offset) {
	m_positionOffset = offset;
}

void LevelDynamicTile::setDynamicTileID(LevelDynamicTileID id) {
	m_dynamicTileID = id;
}

const sf::Vector2f& LevelDynamicTile::getPositionOffset() const {
	return m_positionOffset;
}

LevelDynamicTileID LevelDynamicTile::getDynamicTileID() const {
	return m_dynamicTileID;
}

GameObjectType LevelDynamicTile::getConfiguredType() const {
	return GameObjectType::_DynamicTile;
}

void LevelDynamicTile::onHit(LevelMovableGameObject* mob) {
	// nop
}
