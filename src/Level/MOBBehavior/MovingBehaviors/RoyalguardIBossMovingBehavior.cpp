#include "Level/MOBBehavior/MovingBehaviors/RoyalguardIBossMovingBehavior.h"
#include "Level/Enemies/RoyalguardBoss.h"

RoyalguardIBossMovingBehavior::RoyalguardIBossMovingBehavior(RoyalguardBoss* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy),
	WalkingBehavior(enemy) {
	m_boss = enemy;
}

void RoyalguardIBossMovingBehavior::execHandleMovementInput() {
	if (m_boss->getBossState() == RoyalguardBossState::FireballStart) {
		m_nextIsFacingRight = m_mainChar->getCenter().x > m_enemy->getCenter().x;
	}
	if (m_boss->getBossState() == RoyalguardBossState::FireballStart ||
		m_boss->getBossState() == RoyalguardBossState::Healing ||
		m_boss->getBossState() == RoyalguardBossState::TopFire ||
		m_boss->getBossState() == RoyalguardBossState::BottomFire) {
		m_movingDirectionX = 0;
		m_movingDirectionY = 0;
		return;
	}

	gotoTarget(m_mainChar->getCenter(), m_approachingDistance);
}

void RoyalguardIBossMovingBehavior::updateAnimation(const sf::Time& frameTime) {
	GameObjectState newState = GameObjectState::Idle;
	if (m_enemy->isDead()) {
		newState = GameObjectState::Dead;
	}
	else if (m_boss->getBossState() == RoyalguardBossState::BottomFire) {
		newState = GameObjectState::Casting;
	}
	else if (m_boss->getBossState() == RoyalguardBossState::TopFire) {
		newState = GameObjectState::Casting2;
	}
	else if (m_boss->getBossState() == RoyalguardBossState::Healing || m_boss->getBossState() == RoyalguardBossState::FireballStart) {
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
