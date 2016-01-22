#include "Level/EnemyBehavior/AttackingBehavior.h"
#include "Screens/LevelScreen.h"

AttackingBehavior::AttackingBehavior(Enemy* enemy) {
	m_enemy = enemy;

	m_enemies = enemy->getScreen()->getObjects(GameObjectType::_Enemy);
	m_currentTarget = nullptr;
}

void AttackingBehavior::handleAttackInput() {
	m_attackInput();
}

void AttackingBehavior::update(const sf::Time& frameTime) {
	// nop
}

void AttackingBehavior::setAttackInput(const std::function<void()>& agent) {
	m_attackInput = agent;
}

float AttackingBehavior::getAggroRange() const {
	return m_aggroRange;
}

void AttackingBehavior::setAggroRange(float range) {
	m_aggroRange = range;
}

float AttackingBehavior::distToTarget() const {
	if (m_currentTarget == nullptr) return 10000.f;
	sf::Vector2f dist = m_currentTarget->getCenter() - m_enemy->getCenter();
	return sqrt(dist.x * dist.x + dist.y * dist.y);
}

const LevelMovableGameObject* AttackingBehavior::getCurrentTarget() const {
	return m_currentTarget;
}



