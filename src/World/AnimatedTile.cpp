#include "World/AnimatedTile.h"

GameObjectType AnimatedTile::getConfiguredType() const {
	return GameObjectType::_AnimatedTile;
}

const AnimatedSprite& AnimatedTile::getAnimatedSprite() const {
	return m_animatedSprite;
}