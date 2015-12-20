#pragma once

#include "global.h"
#include "AnimatedGameObject.h"

// a animated tile in level or map
class AnimatedTile : public virtual AnimatedGameObject {
public:
	AnimatedTile() : AnimatedGameObject() {};

	GameObjectType getConfiguredType() const override;
	const AnimatedSprite& getAnimatedSprite() const;
};