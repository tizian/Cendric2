#pragma once

#include "global.h"
#include "Level/EnemyBehavior/AttackingBehavior.h"

// The attacking behavior of the level main character, controlled by user input
class UserAttackingBehavior : public AttackingBehavior {
public:
	UserAttackingBehavior(LevelMainCharacter* mainChar);
	~UserAttackingBehavior() {};
};