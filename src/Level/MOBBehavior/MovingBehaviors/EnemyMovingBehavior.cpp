#include "Level/MOBBehavior/MovingBehaviors/EnemyMovingBehavior.h"

EnemyMovingBehavior::EnemyMovingBehavior(Enemy* enemy) : MovingBehavior(enemy) {
	m_enemy = enemy;
}

EnemyMovingBehavior::~EnemyMovingBehavior() {
	delete m_movingTarget;
}

void EnemyMovingBehavior::setFacingRight(bool value) {
	MovingBehavior::setFacingRight(value);
	m_movingDirectionX = value ? 1 : -1;
}

void EnemyMovingBehavior::handleMovementInput() {
	if (m_movingTarget != nullptr) {
		gotoTarget(*m_movingTarget, 10.f, true, false);
	}
	else {
		execHandleMovementInput();
	}

	handleTrueAcceleration();
}

void EnemyMovingBehavior::gotoTarget(const sf::Vector2f& target, float approachingDistance, bool hold, bool exact) {
	sf::Vector2f center = m_enemy->getCenter();

	// the enemy tries to get near its target
	if (target.x < center.x && std::abs(target.x - center.x) > approachingDistance) {
		m_movingDirectionX = -1;
	}
	else if (target.x > center.x && std::abs(target.x - center.x) > approachingDistance) {
		m_movingDirectionX = 1;
	}
	else {
		m_movingDirectionX = 0;
		if (hold) {
			m_enemy->setVelocity(sf::Vector2f(0.f, m_enemy->getVelocity().y));
			m_enemy->setAccelerationX(0.f);
		}
		if (exact) {
			m_enemy->setPositionX(target.x - (center.x - m_enemy->getPosition().x));
		}
	}

	if (target.y < center.y && std::abs(target.y - center.y) > approachingDistance) {
		m_movingDirectionY = -1;
	}
	else if (target.y > center.y && std::abs(target.y - center.y) > approachingDistance) {
		m_movingDirectionY = 1;
	}
	else {
		m_movingDirectionY = 0;
		if (hold && !m_isWalkingBehavior || exact) {
			m_enemy->setVelocity(sf::Vector2f(m_enemy->getVelocity().x, 0.f));
			m_enemy->setAccelerationY(0.f);
		}
		if (exact) {
			m_enemy->setPositionY(target.y - (center.y - m_enemy->getPosition().y));
		}
	}
}

void EnemyMovingBehavior::setApproachingDistance(float distance) {
	m_approachingDistance = distance;
}

void EnemyMovingBehavior::setMovingTarget(int x, int y) {
	delete m_movingTarget;
	m_movingTarget = new sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
}

void EnemyMovingBehavior::resetMovingTarget() {
	delete m_movingTarget;
	m_movingTarget = nullptr;
	m_movingDirectionX = 0;
	m_movingDirectionY = 0;
	m_enemy->setVelocity(sf::Vector2f(0.f, 0.f));
}
