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
	if (m_currentTarget != nullptr && (m_currentTarget->isDead() || m_currentTarget->isDisposed() || distToTarget() > getAggroRange()) ) {
 		m_currentTarget = nullptr;
	}
	if (m_enemy->getEnemyState() == EnemyState::Idle || m_enemy->getEnemyState() == EnemyState::Waiting) {
		m_currentTarget = nullptr;
	}
	if (m_currentTarget) {
		m_enemy->setAlwaysUpdate();
	}
	if (m_enemy->getEnemyState() == EnemyState::Chasing && !m_currentTarget) {
		m_enemy->setIdle();
	}
	if (m_currentTarget || m_enemy->getEnemyState() != EnemyState::Idle) return;

	// search for new target
	LevelMovableGameObject* nearest = nullptr;
	float nearestDistance = 10000.f;
	if (isInAggroRange(m_mainChar, m_enemy, m_aggroRange)) {
		nearest = m_mainChar;
		nearestDistance = dist(m_mainChar->getCenter(), m_enemy->getCenter());
	}

	// search for new target
	for (auto& go : *m_enemies) {
		if (!go->isViewable()) continue;
		Enemy* enemy = dynamic_cast<Enemy*>(go);
		if (enemy->isDead() || !enemy->isAlly()) continue;
		float distance = dist(go->getCenter(), m_enemy->getCenter());
		if (distance < nearestDistance) {
			nearestDistance = distance;
			nearest = enemy;
		}
	}
	if (nearest == nullptr || nearestDistance > m_aggroRange) {
		m_currentTarget = nullptr;
		m_enemy->setWaiting();
		return;
	}
	m_currentTarget = nearest;
	m_enemy->setChasing();
}