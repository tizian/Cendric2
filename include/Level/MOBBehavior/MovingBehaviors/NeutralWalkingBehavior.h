#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/WalkingBehavior.h"

// A moving behavior for neutral walking enemies.
class NeutralWalkingBehavior : public virtual WalkingBehavior {
public:
	NeutralWalkingBehavior(Enemy* enemy);

	void execHandleMovementInput() override;
};