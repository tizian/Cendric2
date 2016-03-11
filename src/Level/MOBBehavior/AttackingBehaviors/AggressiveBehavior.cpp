#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"

AggressiveBehavior::AggressiveBehavior(Enemy* enemy) : 
    AttackingBehavior(enemy),
    EnemyAttackingBehavior(enemy) {
	m_currentTarget = m_mainChar;
}

sf::Color AggressiveBehavior::getConfiguredHealthColor() const {
	return COLOR_BAD;
} 

void AggressiveBehavior::updateAggro() {
	if (m_enemy->getEnemyState() == EnemyState::Chasing && m_enemy->getFleeCondition()) {
		m_enemy->setFleeing();
		return;
	}
	if (m_currentTarget == nullptr || m_currentTarget->isDead() || m_currentTarget->isDisposed()) {
		m_currentTarget = nullptr;
	}
	if (m_enemy->getEnemyState() != EnemyState::Idle) return;

	bool isInAggroRange = false;

	// handle main character aggro
	float invisibilityScaler = 1.f;
	int invLevel = m_mainChar->getInvisibilityLevel();
	int mentalStr = m_enemy->getMentalStrength();
	if (invLevel == 0) {
		invisibilityScaler = 1.f;
	}
	else if (invLevel > mentalStr) {
		invisibilityScaler = 0.f;
	}
	else {
		invisibilityScaler = 1.f / (2 * (6 - mentalStr)) + 1.f / (invLevel + 1);
	}
	isInAggroRange = distToTarget() < (getAggroRange() * invisibilityScaler);
	if (isInAggroRange) {
		m_enemy->setChasing();
		return;
	}
}