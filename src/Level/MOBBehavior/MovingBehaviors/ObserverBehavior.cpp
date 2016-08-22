#include "Level/MOBBehavior/MovingBehaviors/ObserverBehavior.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"

ObserverBehavior::ObserverBehavior(Enemy* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy) {

	m_isWalkingBehavior = false;
	m_dampingAirPerS = 0.5f;
	m_dampingGroundPerS = 0.7f;
}

void ObserverBehavior::makeRandomDecision() {
	m_movingDirectionX = rand() % 3 - 1;
	m_movingDirectionY = rand() % 3 - 1;
}

void ObserverBehavior::handleDefaultAcceleration() {
	float newAccelerationX = m_mob->getAcceleration().x;
	m_mob->setAcceleration(sf::Vector2f(newAccelerationX, 0.f));
}

void ObserverBehavior::checkCollisions(const sf::Vector2f& nextPosition) {
	// observer's don't care about collisions.
}

void ObserverBehavior::updateAnimation() {
	// nop, state is handled in observer enemy class.
}

void ObserverBehavior::execHandleMovementInput() {
	if (m_enemy->getState() == GameObjectState::Idle) return;

	// movement AI
	sf::Vector2f center = m_enemy->getCenter();
	sf::Vector2f targetCenter = m_mainChar->getCenter();

	float approachingDistance = m_enemy->getState() == GameObjectState::Observing ? m_approachingDistance : 50.f;

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

