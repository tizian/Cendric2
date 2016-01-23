#include "Level/MOBBehavior/AttackingBehavior.h"
#include "Screens/LevelScreen.h"

AttackingBehavior::AttackingBehavior(LevelMovableGameObject* mob) {
	m_mob = mob;
	m_mainChar = dynamic_cast<LevelScreen*>(mob->getScreen())->getMainCharacter();
}

void AttackingBehavior::handleAttackInput() {
	if (m_mob->isDead()) {
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



