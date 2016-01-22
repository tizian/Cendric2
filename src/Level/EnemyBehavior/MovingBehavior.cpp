#include "Level/EnemyBehavior/MovingBehavior.h"

MovingBehavior::MovingBehavior(Enemy* enemy) {
	m_enemy = enemy;
}

void MovingBehavior::setApproachingDistance(float distance) {
	m_approachingDistance = distance;
}

float MovingBehavior::getApproachingDistance() const {
	return m_approachingDistance;
}

void MovingBehavior::update(const sf::Time& frameTime) {
	// nop
}
