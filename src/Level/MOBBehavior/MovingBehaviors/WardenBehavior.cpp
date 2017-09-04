#include "Level/MOBBehavior/MovingBehaviors/WardenBehavior.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"
#include "Level/Enemies/WardenEnemy.h"

WardenBehavior::WardenBehavior(WardenEnemy* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy) {

	m_warden = enemy;
	m_dampingAirPerS = 0.5f;
	m_dampingGroundPerS = 0.7f;
	m_ignoreDynamicTiles = true;
	m_isWalkingBehavior = false;
}

void WardenBehavior::makeRandomDecision() {
	m_movingDirectionX = rand() % 3 - 1;
	m_movingDirectionY = rand() % 3 - 1;
}

void WardenBehavior::handleDefaultAcceleration() {
	float newAccelerationX = m_mob->getAcceleration().x;
	m_mob->setAcceleration(sf::Vector2f(newAccelerationX, 0.f));
}

void WardenBehavior::handleTrueAcceleration() {
	float newAccelerationY = m_movingDirectionY * m_walkAcceleration;
	float newAccelerationX = m_movingDirectionX * m_walkAcceleration;

	m_enemy->setAcceleration(sf::Vector2f(newAccelerationX, newAccelerationY));
}

void WardenBehavior::checkCollisions(const sf::Vector2f& nextPosition) {
	// wardens don't care about collisions.
}

void WardenBehavior::updateAnimation(const sf::Time& frameTime) {
	updateTime(m_remainingAnimationTime, frameTime);

	if (m_remainingAnimationTime > sf::Time::Zero) return;

	// switch animation
	if (m_isIdle) {
		bool chooseLooking = rand() % 2 == 1;
		if (chooseLooking) {
			m_remainingAnimationTime = m_warden->getAnimation(GameObjectState::Looking)->getAnimationTime();
			m_warden->setState(GameObjectState::Looking);
		}
		else {
			m_remainingAnimationTime = m_warden->getAnimation(GameObjectState::Blinking)->getAnimationTime();
			m_warden->setState(GameObjectState::Blinking);
		}
	}
	else {
		m_remainingAnimationTime = getIdleTime();
		m_warden->setState(GameObjectState::Idle);
	}
	m_isIdle = !m_isIdle;
}

sf::Time WardenBehavior::getIdleTime() const {
	int milliseconds = rand() % 4000 + 1000;
	return sf::milliseconds(milliseconds);
}

void WardenBehavior::execHandleMovementInput() {
	if (m_warden->getWardenState() == WardenState::Idle) return;

	// movement AI
	sf::Vector2f center = m_enemy->getCenter();
	sf::Vector2f targetCenter = m_mainChar->getCenter();

	float approachingDistance = m_warden->getWardenState() == WardenState::Observing ? m_approachingDistance : 50.f;

	if (targetCenter.x < center.x && std::abs(targetCenter.x - center.x) > approachingDistance) {
		m_movingDirectionX = -1;
	}
	else if (targetCenter.x > center.x && std::abs(targetCenter.x - center.x) > approachingDistance) {
		m_movingDirectionX = 1;
	}
	else {
		m_movingDirectionX = 0;
	}

	if (targetCenter.y < center.y && std::abs(targetCenter.y - center.y) > approachingDistance) {
		m_movingDirectionY = -1;
	}
	else if (targetCenter.y > center.y && std::abs(targetCenter.y - center.y) > approachingDistance) {
		m_movingDirectionY = 1;
	}
	else {
		m_movingDirectionY = 0;
	}
}

