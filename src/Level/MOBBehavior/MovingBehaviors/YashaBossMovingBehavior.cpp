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
	// movement AI
	if (m_enemy->getEnemyState() == EnemyState::Chasing) {

		sf::Vector2f center = m_enemy->getCenter();
		sf::Vector2f targetCenter = m_mainChar->getCenter();

		if (targetCenter.x < center.x && std::abs(targetCenter.x - center.x) > m_approachingDistance) {
			m_movingDirectionX = -1;
		}
		else if (targetCenter.x > center.x && std::abs(targetCenter.x - center.x) > m_approachingDistance) {
			m_movingDirectionX = 1;
		}
		else {
			m_movingDirectionX = 0;
		}

		if (targetCenter.y < center.y && std::abs(targetCenter.y - center.y) > m_approachingDistance) {
			m_movingDirectionY = -1;
		}
		else if (targetCenter.y > center.y && std::abs(targetCenter.y - center.y) > m_approachingDistance) {
			m_movingDirectionY = 1;
		}
		else {
			m_movingDirectionY = 0;
		}
	}
}

void YashaBossMovingBehavior::updateAnimation(const sf::Time& frameTime) {
	FlyingBehavior::updateAnimation(frameTime);
}
