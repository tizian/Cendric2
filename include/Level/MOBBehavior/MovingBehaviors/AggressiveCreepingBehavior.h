#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/CreepingBehavior.h"

// A moving behavior for aggressive creeping enemies.
class AggressiveCreepingBehavior final : public CreepingBehavior {
public:
	AggressiveCreepingBehavior(Enemy* enemy);

	void execHandleMovementInput() override;
};