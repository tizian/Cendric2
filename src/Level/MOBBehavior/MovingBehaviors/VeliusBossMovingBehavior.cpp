#include "Level/MOBBehavior/MovingBehaviors/VeliusBossMovingBehavior.h"
#include "Level/Enemies/VeliusBoss.h"

VeliusBossMovingBehavior::VeliusBossMovingBehavior(VeliusBoss* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy),
	WalkingBehavior(enemy) {
	m_boss = enemy;
}

void VeliusBossMovingBehavior::execHandleMovementInput() {
	// todo
	//gotoTarget(m_mainChar->getCenter(), m_approachingDistance);
}
