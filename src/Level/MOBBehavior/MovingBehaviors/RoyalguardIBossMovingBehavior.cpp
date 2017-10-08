#include "Level/MOBBehavior/MovingBehaviors/RoyalguardIBossMovingBehavior.h"
#include "Level/Enemies/RoyalguardBoss.h"

RoyalguardIBossMovingBehavior::RoyalguardIBossMovingBehavior(RoyalguardBoss* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy),
	WalkingBehavior(enemy) {
	m_boss = enemy;
}

void RoyalguardIBossMovingBehavior::execHandleMovementInput() {
	/*if (m_enemy->getState() == GameObjectState::Walking && m_enemy->getActiveSpellCount() == 0) {
	// we hit our target.
	setReady();
	m_movingDirectionX = 0;
	m_enemy->setState(GameObjectState::Idle);
	}*/
}

void RoyalguardIBossMovingBehavior::checkCollisions(const sf::Vector2f& nextPosition) {
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

		m_enemy->clearSpells(true);
		m_fightAnimationTime = sf::Time::Zero;
	}
}

void RoyalguardIBossMovingBehavior::update(const sf::Time& frameTime) {
	WalkingBehavior::update(frameTime);
}

void RoyalguardIBossMovingBehavior::updateAnimation(const sf::Time& frameTime) {
	WalkingBehavior::updateAnimation(frameTime);
}
