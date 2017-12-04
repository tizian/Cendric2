#include "Level/MOBBehavior/MovingBehaviors/VeliusVictimMovingBehavior.h"
#include "Level/Enemies/VeliusVictim.h"

const sf::Vector2f VeliusVictimMovingBehavior::ALTAR_POS = sf::Vector2f(300.f, 600.f);

VeliusVictimMovingBehavior::VeliusVictimMovingBehavior(VeliusVictim* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy),
	WalkingBehavior(enemy) {
	m_victim = enemy;
}

void VeliusVictimMovingBehavior::execHandleMovementInput() {
	if (!m_isCalled) return;
	if (m_isAtAltar) return;

	gotoTarget(ALTAR_POS, 10.f, true, true);

	if (dist(ALTAR_POS, m_victim->getPosition()) < 10.f) {
		m_isAtAltar = true;
	}
}
