#include "Level/MOBBehavior/MovingBehaviors/RoyalguardTBossMovingBehavior.h"
#include "Level/Enemies/RoyalguardBoss.h"

const float RoyalguardTBossMovingBehavior::CHARGING_SPEED = 1000.f;

RoyalguardTBossMovingBehavior::RoyalguardTBossMovingBehavior(RoyalguardBoss* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy),
	WalkingBehavior(enemy) {
	m_boss = enemy;
}

void RoyalguardTBossMovingBehavior::setChargingDirection(const sf::Vector2f& direction) {
	m_chargingDirection = direction;
}

void RoyalguardTBossMovingBehavior::execHandleMovementInput() {
	if (m_boss->getBossState() == RoyalguardBossState::ChargingStart) {
		m_nextIsFacingRight = m_mainChar->getCenter().x > m_enemy->getCenter().x;
		m_movingDirectionX = 0;
		m_movingDirectionY = 0;
		return;
	}
	if (m_boss->getBossState() == RoyalguardBossState::Charging) {
		m_nextIsFacingRight = m_chargingDirection.x >= 0.f;
		m_movingDirectionX = m_nextIsFacingRight ? 1 : -1;
		m_movingDirectionY = m_chargingDirection.y >= 0.f ? 1 : -1;
		m_boss->setVelocity(m_chargingDirection * CHARGING_SPEED);
		m_gravity = 0.f;
		return;
	}
	m_gravity = 1000.f;
	if (m_boss->getBossState() == RoyalguardBossState::Healing) {
		m_movingDirectionX = 0;
		m_movingDirectionY = 0;
		return;
	}

	gotoTarget(m_mainChar->getCenter(), m_approachingDistance);
}

void RoyalguardTBossMovingBehavior::checkCollisions(const sf::Vector2f& nextPosition) {
	float oldPositionX = m_enemy->getPosition().x;

	checkXYDirection(nextPosition, m_isCollidingX, m_isCollidingY);
	sf::FloatRect nextBoundingBox(nextPosition.x, nextPosition.y, m_mob->getBoundingBox()->width, m_mob->getBoundingBox()->height);

	if (m_boss->getBossState() == RoyalguardBossState::Charging && (m_isCollidingX || m_isCollidingY)) {
		m_movingDirectionX = 0;
		m_enemy->setAccelerationX(0.f);
		m_enemy->setVelocityX(0.f);
		m_enemy->setPositionX(oldPositionX);

		m_enemy->clearSpells(true);
		m_boss->clearBossState();
	}
}

void RoyalguardTBossMovingBehavior::updateAnimation(const sf::Time& frameTime) {
	GameObjectState newState = GameObjectState::Idle;
	if (m_enemy->isDead()) {
		newState = GameObjectState::Dead;
	}
	else if (m_boss->getBossState() == RoyalguardBossState::ChargingStart) {
		newState = GameObjectState::Casting2;
	}
	else if (m_boss->getBossState() == RoyalguardBossState::Charging) {
		newState = GameObjectState::Fighting2;
	}
	else if (m_boss->getBossState() == RoyalguardBossState::Healing) {
		newState = GameObjectState::Fighting3;
	}
	else if (m_fightAnimationTime > sf::Time::Zero) {
		newState = m_fightAnimationState;
	}
	else if (!m_isGrounded) {
		newState = GameObjectState::Jumping;
	}
	else if (std::abs(m_enemy->getVelocity().x) > 20.f) {
		newState = GameObjectState::Walking;
	}

	// only update animation if we need to
	if (m_enemy->getState() != newState || (m_nextIsFacingRight != m_isFacingRight)) {
		m_isFacingRight = m_nextIsFacingRight;
		m_enemy->setState(newState);
		m_enemy->setCurrentAnimation(m_enemy->getAnimation(m_enemy->getState()), !m_isFacingRight);
	}
}
