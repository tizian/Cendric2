#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/FlyingBehavior.h"

// A moving behavior for aggressive flying enemies.
class AggressiveFlyingBehavior final : public FlyingBehavior {
public:
	AggressiveFlyingBehavior(Enemy* enemy);

	void execHandleMovementInput() override;
};