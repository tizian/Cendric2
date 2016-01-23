#include "Level/EnemyBehavior/UserAttackingBehavior.h"

UserAttackingBehavior::UserAttackingBehavior(LevelMainCharacter* mainChar) :
	AttackingBehavior(mainChar) {
	// use this assignment because the "normal" assigner in moving behavior can't get this yet.
	m_mainChar = mainChar;
}