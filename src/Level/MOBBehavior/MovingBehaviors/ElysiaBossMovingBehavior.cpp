#include "Level/MOBBehavior/MovingBehaviors/ElysiaBossMovingBehavior.h"
#include "Level/Enemies/ElysiaBoss.h"

const sf::Time ElysiaBossMovingBehavior::PROJECTILE_TIME = sf::seconds(10.f);

ElysiaBossMovingBehavior::ElysiaBossMovingBehavior(ElysiaBoss* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy),
	FlyingBehavior(enemy) {
	m_boss = enemy;
	m_timeUntilTransition = PROJECTILE_TIME;

	// calculate targets
	float noseTargetHeight = 500.f;
	float thunderHeight = 200.f;
	float levelWidth = m_enemy->getLevel()->getWorldRect().width;
	m_noseTargetLeft = sf::Vector2f(200.f, noseTargetHeight);
	m_noseTargetRight = sf::Vector2f(levelWidth - 100.f - enemy->getBoundingBox()->width, noseTargetHeight);
	m_thunderTarget = sf::Vector2f(levelWidth * 0.5f, thunderHeight);
}

void ElysiaBossMovingBehavior::execHandleMovementInput() {
	switch (m_boss->getBossState()) {
	case ElysiaBossState::Projectile:
		handleProjectileMovement();
		return;
	case ElysiaBossState::PreThunderstorm:
		handleThunderUp();
		return;
	case ElysiaBossState::PreNosedive:
		handleNosediveUp();
		return;
	case ElysiaBossState::Nosedive:
		handleNosediveDown();
		return;
	default:
		return;
	}
}

void ElysiaBossMovingBehavior::handleFlyToTarget(float appDist, const sf::Vector2f& target) {
	sf::Vector2f center = m_enemy->getCenter();

	if (target.x < center.x && std::abs(target.x - center.x) > appDist) {
		m_movingDirectionX = -1;
	}
	else if (target.x > center.x && std::abs(target.x - center.x) > appDist) {
		m_movingDirectionX = 1;
	}
	else {
		m_movingDirectionX = 0;
	}

	if (target.y < center.y && std::abs(target.y - center.y) > appDist) {
		m_movingDirectionY = -1;
	}
	else if (target.y > center.y && std::abs(target.y - center.y) > appDist) {
		m_movingDirectionY = 1;
	}
	else {
		m_movingDirectionY = 0;
	}
}

void ElysiaBossMovingBehavior::handleProjectileMovement() {
	handleFlyToTarget(m_approachingDistance, m_mainChar->getCenter());
}

void ElysiaBossMovingBehavior::handleNosediveDown() {
	// SINUS COSINUS TANGENS
}

void ElysiaBossMovingBehavior::handleNosediveUp() {
	handleFlyToTarget(20.f, m_flyingTarget);
}

void ElysiaBossMovingBehavior::handleThunderUp() {
	handleFlyToTarget(20.f, m_flyingTarget);
}

void ElysiaBossMovingBehavior::update(const sf::Time& frameTime) {
	updateBossState(frameTime);
	FlyingBehavior::update(frameTime);
}

void ElysiaBossMovingBehavior::updateBossState(const sf::Time& frameTime) {
	switch (m_boss->getBossState()) {
	case ElysiaBossState::Projectile:
		updateTime(m_timeUntilTransition, frameTime);
		if (m_timeUntilTransition == sf::Time::Zero) {
			if (rand() % 100 > 70) {
				// thunderstorm
				m_boss->setBossState(ElysiaBossState::PreThunderstorm);
				m_flyingTarget = m_thunderTarget;
			}
			else {
				m_boss->setBossState(ElysiaBossState::PreNosedive);
				m_isNoseRight = rand() % 2 == 0;
				m_flyingTarget = m_isNoseRight ? m_noseTargetRight : m_noseTargetLeft;
			}
		}
		break;
	default:
		break;
	}

	if (m_boss->getBossState() != ElysiaBossState::Projectile) return;
	

	
}



