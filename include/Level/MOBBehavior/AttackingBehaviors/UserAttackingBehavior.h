#pragma once

#include "global.h"
#include "Level/MOBBehavior/AttackingBehavior.h"

// The attacking behavior of the level main character, controlled by user input
class UserAttackingBehavior final : public AttackingBehavior {
public:
	UserAttackingBehavior(LevelMainCharacter* mainChar);
};