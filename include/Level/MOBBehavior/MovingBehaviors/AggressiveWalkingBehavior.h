#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/WalkingBehavior.h"

// A moving behavior for aggressive walking enemies.
class AggressiveWalkingBehavior final : public virtual WalkingBehavior {
public:
	AggressiveWalkingBehavior(Enemy* enemy);

	void execHandleMovementInput() override;
};