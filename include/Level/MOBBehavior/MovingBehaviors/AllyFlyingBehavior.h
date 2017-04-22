#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/FlyingBehavior.h"

// A moving behavior for allied flying enemies.
class AllyFlyingBehavior final : public virtual FlyingBehavior {
public:
	AllyFlyingBehavior(Enemy* enemy);

	void execHandleMovementInput() override;
};