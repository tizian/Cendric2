#pragma once

#include "global.h"
#include "World/AnimatedGameObject.h"

// a animated tile in level or map
class AnimatedTile final : public AnimatedGameObject {
public:
	AnimatedTile() : AnimatedGameObject() {};

	GameObjectType getConfiguredType() const override;
	const AnimatedSprite& getAnimatedSprite() const;
};