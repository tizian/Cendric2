#include "Level/MOBBehavior/AttackingBehavior.h"
#include "Level/MOBBehavior/MovingBehavior.h"
#include "Level/Enemy.h"
#include "Screens/LevelScreen.h"

AttackingBehavior::AttackingBehavior(LevelMovableGameObject* mob) {
	m_mob = mob;
	m_mainChar = dynamic_cast<LevelScreen*>(mob->getScreen())->getMainCharacter();
}

void AttackingBehavior::handleAttackInput() {
	if (m_mob->isDead() || (m_mob->getMovingBehavior() && !m_mob->getMovingBehavior()->isReady())) {
		return;
	}
	m_executeAttack();
}

void AttackingBehavior::update(const sf::Time& frameTime) {
	handleAttackInput();
}

void AttackingBehavior::setAttackInput(const std::function<void()>& agent) {
	m_executeAttack = agent;
}

bool AttackingBehavior::isInAggroRange(const LevelMainCharacter* mainChar, const Enemy* enemy, float aggroRange) {
	float invisibilityScaler = 1.f;
	int invLevel = mainChar->getInvisibilityLevel();
	int mentalStr = enemy->getMentalStrength();
	if (invLevel == 0) {
		invisibilityScaler = 1.f;
	}
	else if (invLevel > mentalStr) {
		invisibilityScaler = 0.f;
	}
	else {
		invisibilityScaler = 1.f / (2 * (6 - mentalStr)) + 1.f / (invLevel + 1);
	}

	return dist(enemy->getCenter(), mainChar->getCenter()) < (aggroRange * invisibilityScaler);
}