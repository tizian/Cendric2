#include "Level/MOBBehavior/AttackingBehaviors/NeutralBehavior.h"

NeutralBehavior::NeutralBehavior(Enemy* enemy) : 
	EnemyAttackingBehavior(enemy),
	AttackingBehavior(enemy) {
}

sf::Color NeutralBehavior::getConfiguredHealthColor() const {
	return sf::Color::Yellow;
}

void NeutralBehavior::updateAggro() {
	// TODO: check for stealing cendric.
}