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

	if (AttackingBehavior::isInAggroRange(m_mainChar, m_enemy, m_aggroRange)) {
		m_enemy->setChasing();
		return;
	}
}