#include "World/AnimatedTile.h"

GameObjectType AnimatedTile::getConfiguredType() const {
	return _AnimatedTile;
}

const AnimatedSprite& AnimatedTile::getAnimatedSprite() const {
	return m_animatedSprite;
}