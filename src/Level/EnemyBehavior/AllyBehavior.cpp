#include "Level/EnemyBehavior/AllyBehavior.h"
#include "Level/LevelMainCharacter.h"

void AllyBehavior::update(const sf::Time& frameTime) {
	GameObject::updateTime(m_timeToLive, frameTime);
	if (m_timeToLive == sf::Time::Zero) {
		m_enemy->setDead();
	}
}

void AllyBehavior::setTimeToLive(const sf::Time& ttl) {
	m_timeToLive = ttl;
}

EnemyAttitude AllyBehavior::getAttitude() const {
	return EnemyAttitude::Ally;
}

sf::Color AllyBehavior::getConfiguredHealthColor() const {
	return sf::Color::Green;
}

void AllyBehavior::updateAggro() {
	if (m_enemy->m_enemyState == EnemyState::Chasing && m_enemy->getFleeCondition()) {
		m_enemy->m_fearedTime = m_enemy->getConfiguredFearedTime();
		return;
	}
	if (m_currentTarget == nullptr || m_currentTarget->isDead() || m_currentTarget->isDisposed()) {
		m_currentTarget = nullptr;
	}
	if (m_enemy->m_enemyState != EnemyState::Idle) return;

	bool isInAggroRange = false;

	// search for new target
	Enemy* nearest = nullptr;
	float nearestDistance = 10000.f;
	for (auto& go : *m_enemies) {
		if (!go->isViewable()) continue;
		Enemy* enemy = dynamic_cast<Enemy*>(go);
		if (enemy->isDead() || enemy->isAlly()) continue;
		sf::Vector2f dist = go->getCenter() - m_enemy->getCenter();
		float distance = sqrt(dist.x * dist.x + dist.y * dist.y);
		if (distance < nearestDistance) {
			nearestDistance = distance;
			nearest = enemy;
		}
	}
	if (nearest == nullptr) {
		m_currentTarget = nullptr;
		m_enemy->m_waitingTime = m_enemy->getConfiguredWaitingTime();
		return;
	}
	m_currentTarget = nearest;
	m_enemy->m_chasingTime = m_enemy->getConfiguredChasingTime();
}

