#include "Level/MOBBehavior/MovingBehaviors/VeliusBossMovingBehavior.h"
#include "Level/Enemies/VeliusBoss.h"

VeliusBossMovingBehavior::VeliusBossMovingBehavior(VeliusBoss* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy),
	WalkingBehavior(enemy) {
	m_boss = enemy;
	setFacingRight(false);
}

void VeliusBossMovingBehavior::execHandleMovementInput() {
	if (m_boss->isBlocked()) {
		stopAll();
		return;
	}
	
	if (dist(m_mainChar->getCenter(), m_mob->getCenter()) > m_approachingDistance + 1.f) {
		gotoTarget(m_mainChar->getCenter(), m_approachingDistance);
	}
}

void VeliusBossMovingBehavior::updateAnimation(const sf::Time& frameTime) {
	GameObjectState newState = GameObjectState::Idle;
	auto state = m_mob->getState();
	if (state == GameObjectState::Casting3 || state == GameObjectState::Broken) {
		newState = state;
		setFacingRight(false);
	}
	else if (m_enemy->isDead()) {
		newState = GameObjectState::Dead;
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
