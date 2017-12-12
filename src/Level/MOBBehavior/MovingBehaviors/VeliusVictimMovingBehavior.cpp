#include "Level/MOBBehavior/MovingBehaviors/VeliusVictimMovingBehavior.h"
#include "Level/Enemies/VeliusVictim.h"

const sf::Vector2f VeliusVictimMovingBehavior::ALTAR_POS = sf::Vector2f(655.f, 510.f);

VeliusVictimMovingBehavior::VeliusVictimMovingBehavior(VeliusVictim* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy),
	WalkingBehavior(enemy) {
	m_victim = enemy;

	m_initialPos = m_victim->getCenter();
	m_isReturning = false;
}

void VeliusVictimMovingBehavior::execHandleMovementInput() {
	auto state = m_mob->getState();
	if (state == GameObjectState::Inactive || state == GameObjectState::Broken) {
		stopAll();
		return;
	}
	
	if (!m_isReturning) {
		gotoTarget(ALTAR_POS, 2.f, true, true);

		if (dist(ALTAR_POS, m_victim->getCenter()) < 4.f) {
			m_mob->setPosition(ALTAR_POS);
			m_mob->setState(GameObjectState::Broken);
		}
	}
	else {
		gotoTarget(m_initialPos, 2.f, true, true);

		if (dist(m_initialPos, m_victim->getCenter()) < 4.f) {
			m_mob->setPosition(m_initialPos);
			m_mob->setFacingRight(true);
			m_mob->setState(GameObjectState::Inactive);
		}
	}
}

void VeliusVictimMovingBehavior::updateAnimation(const sf::Time& frameTime) {
	GameObjectState newState = GameObjectState::Idle;
	auto state = m_mob->getState();
	if (state == GameObjectState::Inactive || state == GameObjectState::Broken) {
		newState = state;
	}
	else if (m_enemy->isDead()) {
		newState = GameObjectState::Dead;
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

void VeliusVictimMovingBehavior::callToDie() {
	m_mob->setState(GameObjectState::Idle);
	m_isReturning = false;
}

void VeliusVictimMovingBehavior::release() {
	m_mob->setState(GameObjectState::Idle);
	m_isReturning = true;
}
