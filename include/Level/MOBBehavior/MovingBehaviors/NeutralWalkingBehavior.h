#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/WalkingBehavior.h"

// A moving behavior for neutral walking enemies.
class NeutralWalkingBehavior final : public WalkingBehavior {
public:
	NeutralWalkingBehavior(Enemy* enemy);

	void execHandleMovementInput() override;
};