#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/FlyingBehavior.h"

// A moving behavior for aggressive flying enemies.
class AggressiveFlyingBehavior : public virtual FlyingBehavior {
public:
	AggressiveFlyingBehavior(Enemy* enemy);

	void handleMovementInput() override;
};