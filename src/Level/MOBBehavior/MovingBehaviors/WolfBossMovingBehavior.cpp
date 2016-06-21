#include "Level/MOBBehavior/MovingBehaviors/WolfBossMovingBehavior.h"

const sf::Time TRIPOVER_TIME = sf::milliseconds(500);
const sf::Time LAYING_TIME = sf::seconds(5.f);
const sf::Time STANDUP_TIME = sf::milliseconds(500);

WolfBossMovingBehavior::WolfBossMovingBehavior(Enemy* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy),
	WalkingBehavior(enemy) {
}

void WolfBossMovingBehavior::execHandleMovementInput() {
	if (m_enemy->getState() == GameObjectState::Casting && m_chargeMovingDirection == 0) {
		m_chargeMovingDirection = m_movingDirectionX;
		m_movingDirectionX = 0;
	}
	else if (m_enemy->getState() == GameObjectState::Walking && m_movingDirectionX == 0) {
		m_movingDirectionX = m_chargeMovingDirection;
		m_chargeMovingDirection = 0;
	}
}

void WolfBossMovingBehavior::checkCollisions(const sf::Vector2f& nextPosition) {
	float oldPositionX = m_enemy->getPosition().x;

	bool collidesY;
	MovingBehavior::checkXYDirection(nextPosition, m_collidesX, collidesY);
	sf::FloatRect nextBoundingBox(nextPosition.x, nextPosition.y, m_mob->getBoundingBox()->width, m_mob->getBoundingBox()->height);

	m_jumps = false;
	if (m_collidesX) {
		m_movingDirectionX = 0;
		m_enemy->setAccelerationX(0.f);
		m_enemy->setVelocityX(0.f);
		m_enemy->setPositionX(oldPositionX);

		m_tripOverTime = TRIPOVER_TIME;
		m_enemy->clearSpells(true);
		m_fightAnimationTime = sf::Time::Zero;
	}
}


void WolfBossMovingBehavior::update(const sf::Time& frameTime) {
	if (m_tripOverTime > sf::Time::Zero) {
		updateTime(m_tripOverTime, frameTime);
		if (m_tripOverTime == sf::Time::Zero) {
			m_layingTime = LAYING_TIME;
			m_enemy->setInvincible(false);
		}
	}
	else if (m_layingTime > sf::Time::Zero) {
		updateTime(m_layingTime, frameTime);
		if (m_layingTime == sf::Time::Zero) {
			m_standupTime = STANDUP_TIME;
			m_enemy->setInvincible(true);
		}
	}
	updateTime(m_standupTime, frameTime);
	WalkingBehavior::update(frameTime);
}


void WolfBossMovingBehavior::updateAnimation() {
	// calculate new game state and set animation.

	GameObjectState newState = GameObjectState::Idle;
	if (m_enemy->isDead()) {
		newState = GameObjectState::Dead;
	}
	else if (m_tripOverTime > sf::Time::Zero) {
		newState = GameObjectState::TripOver;
	}
	else if (m_layingTime > sf::Time::Zero) {
		newState = GameObjectState::Laying;
	}
	else if (m_standupTime > sf::Time::Zero) {
		newState = GameObjectState::Standup;
	}
	else if (m_fightAnimationTime > sf::Time::Zero) {
		newState = m_fightAnimationState;
	}

	// only update animation if we need to
	if (m_enemy->getState() != newState || (m_nextIsFacingRight != m_isFacingRight)) {
		m_isFacingRight = m_nextIsFacingRight;
		m_enemy->setState(newState);
		m_enemy->setCurrentAnimation(m_enemy->getAnimation(m_enemy->getState()), !m_isFacingRight);
	}
}




