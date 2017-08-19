#include "Level/MOBBehavior/MovingBehaviors/YashaBossMovingBehavior.h"
#include "Level/Enemies/YashaBoss.h"
#include "Screens/LevelScreen.h"
#include "ObjectFactory.h"

YashaBossMovingBehavior::YashaBossMovingBehavior(YashaBoss* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy),
	FlyingBehavior(enemy) {
	m_boss = enemy;
}

YashaBossMovingBehavior::~YashaBossMovingBehavior() {
	g_resourceManager->deleteUniqueResources(this);
}

void YashaBossMovingBehavior::update(const sf::Time& frameTime) {
	FlyingBehavior::update(frameTime);
}

void YashaBossMovingBehavior::execHandleMovementInput() {
	switch (m_boss->m_bossState)
	{
	case YashaBossState::GotoExplosion:
		gotoTarget(YashaBoss::ROOM_MID, 10.f, true, true);
		if (dist(m_enemy->getCenter(), YashaBoss::ROOM_MID) < 1.f) {
			m_boss->startBossState(YashaBossState::Explosion);
		}
		break;
	case YashaBossState::GotoStartCat:
		gotoTarget(YashaBoss::ROOM_MID, 10.f, true, true);
		if (dist(m_enemy->getCenter(), YashaBoss::ROOM_MID) < 1.f) {
			m_boss->startBossState(YashaBossState::StartCat);
		}
		break;
	case YashaBossState::Fireballing:
		gotoTarget(m_mainChar->getCenter(), m_approachingDistance, false, false);
		break;
	case YashaBossState::Explosion:
	case YashaBossState::StartCat:
	case YashaBossState::Cat:
		m_movingDirectionX = 0;
		m_movingDirectionY = 0;
	default:
		break;
	}
}

void YashaBossMovingBehavior::updateAnimation(const sf::Time& frameTime) {
	GameObjectState newState = GameObjectState::Idle;
	if (m_boss->m_bossState == YashaBossState::StartCat || m_boss->m_bossState == YashaBossState::Cat) {
		newState = GameObjectState::Fighting;
	}
	else if (m_fightAnimationTime > sf::Time::Zero) {
		newState = m_fightAnimationState;
	}
	else if (std::abs(m_enemy->getVelocity().x) > 20.f || std::abs(m_enemy->getVelocity().y) > 20.f) {
		newState = GameObjectState::Flying;
	}

	// only update animation if we need to
	if (m_enemy->getState() != newState || m_nextIsFacingRight != m_isFacingRight) {
		m_isFacingRight = m_nextIsFacingRight;
		m_enemy->setState(newState);
		m_enemy->setCurrentAnimation(m_enemy->getAnimation(m_enemy->getState()), !m_isFacingRight);
	}
}
