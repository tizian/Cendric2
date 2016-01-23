#include "Level/EnemyBehavior/EnemyMovingBehavior.h"

EnemyMovingBehavior::EnemyMovingBehavior(Enemy* enemy) : MovingBehavior(enemy) {
	m_enemy = enemy;
}

void EnemyMovingBehavior::setApproachingDistance(float distance) {
	m_approachingDistance = distance;
}
