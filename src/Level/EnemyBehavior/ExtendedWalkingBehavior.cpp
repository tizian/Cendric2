#include "Level/EnemyBehavior/ExtendedWalkingBehavior.h"

ExtendedWalkingBehavior::ExtendedWalkingBehavior(Enemy* enemy) :
WalkingBehavior(enemy),
EnemyMovingBehavior(enemy),
MovingBehavior(enemy) {
	
}

void ExtendedWalkingBehavior::update(const sf::Time& frameTime) {
	WalkingBehavior::update(frameTime);
	GameObject::updateTime(m_fightStartTime, frameTime);
	GameObject::updateTime(m_jumpStartTime, frameTime);
}

void ExtendedWalkingBehavior::updateAnimation() {
	// handle the additional states
	GameObjectState newState = GameObjectState::Idle;
	if (m_enemy->isDead()) {
		newState = GameObjectState::Dead;
	}
	else if (m_fightAnimationTime > sf::Time::Zero) {
		if (m_enemy->getState() == GameObjectState::Fighting) {
			newState = GameObjectState::Fighting;
		}
		else {
			if (m_fightStartTime == sf::Time::Zero) {
				m_fightStartTime = m_configuredFightStartTime;
				newState = GameObjectState::Fighting_start;
			}
			else {
				if (m_fightStartTime == sf::Time::Zero) {
					newState = GameObjectState::Fighting;
				}
				else {
					newState = GameObjectState::Fighting_start;
				}
			}
		}
	}
	else if (!m_isGrounded) {
		if (m_enemy->getState() == GameObjectState::Jumping) {
			newState = GameObjectState::Jumping;
		}
		else {
			if (m_jumpStartTime == sf::Time::Zero) {
				m_jumpStartTime = m_configuredJumpStartTime;
				newState = GameObjectState::Jumping_start;
			}
			else {
				if (m_jumpStartTime == sf::Time::Zero) {
					newState = GameObjectState::Jumping;
				}
				else {
					newState = GameObjectState::Jumping_start;
				}
			}
		}
	}
	else if (std::abs(m_enemy->getVelocity().x) > 20.0f) {
		newState = GameObjectState::Walking;
	}

	// only update animation if we need to
	if (m_enemy->getState() != newState || m_nextIsFacingRight != m_isFacingRight) {
		m_isFacingRight = m_nextIsFacingRight;
		m_enemy->setState(newState);
		m_enemy->setCurrentAnimation(m_enemy->getAnimation(m_enemy->getState()), !m_isFacingRight);
	}
}

void ExtendedWalkingBehavior::setJumpStartTime(const sf::Time& time) {
	m_configuredJumpStartTime = time;

}
void ExtendedWalkingBehavior::setFightStartTime(const sf::Time& time) {
	m_configuredFightStartTime = time;
}