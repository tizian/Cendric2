#include "Level/MOBBehavior/AttackingBehaviors/EnemyAttackingBehavior.h"
#include "Screens/LevelScreen.h"

EnemyAttackingBehavior::EnemyAttackingBehavior(Enemy* enemy) : AttackingBehavior(enemy) {
	m_enemy = enemy;

	m_enemies = enemy->getScreen()->getObjects(GameObjectType::_Enemy);
	m_currentTarget = nullptr;
}

void EnemyAttackingBehavior::update(const sf::Time& frameTime) {
	AttackingBehavior::update(frameTime);
	updateAggro();
}

void EnemyAttackingBehavior::handleAttackInput() {
	if (m_enemy->getEnemyState() == EnemyState::Recovering)
		return;

	AttackingBehavior::handleAttackInput();
}

float EnemyAttackingBehavior::getAggroRange() const {
	return m_aggroRange;
}

void EnemyAttackingBehavior::setAggroRange(float range) {
	m_aggroRange = range;
}

float EnemyAttackingBehavior::distToTarget() const {
	if (m_currentTarget == nullptr) return 10000.f;
	sf::Vector2f dist = m_currentTarget->getCenter() - m_mob->getCenter();
	return sqrt(dist.x * dist.x + dist.y * dist.y);
}

const LevelMovableGameObject* EnemyAttackingBehavior::getCurrentTarget() const {
	return m_currentTarget;
}



