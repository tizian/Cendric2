#include "Level/EnemyBehavior/AggressiveBehavior.h"
#include "Level/LevelMainCharacter.h"

EnemyAttitude AggressiveBehavior::getAttitude() const {
	return EnemyAttitude::Aggressive;
}

sf::Color AggressiveBehavior::getConfiguredHealthColor() const {
	return sf::Color::Red;
} 

void AggressiveBehavior::updateAggro() {
	if (m_enemy->m_enemyState == EnemyState::Chasing && m_enemy->getFleeCondition()) {
		m_enemy->m_fearedTime = m_enemy->getConfiguredFearedTime();
		return;
	}
	if (m_currentTarget == nullptr || m_currentTarget->isDead() || m_currentTarget->isDisposed()) {
		m_currentTarget = nullptr;
	}
	if (m_enemy->m_enemyState != EnemyState::Idle) return;

	bool isInAggroRange = false;

	// handle main character aggro
	float invisibilityScaler = 1.f;
	int invLevel = m_enemy->m_mainChar->getInvisibilityLevel();
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
		m_enemy->m_chasingTime = m_enemy->getConfiguredChasingTime();
		return;
	}
}