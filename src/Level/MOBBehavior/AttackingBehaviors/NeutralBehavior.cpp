#include "Level/MOBBehavior/AttackingBehaviors/NeutralBehavior.h"

NeutralBehavior::NeutralBehavior(Enemy* enemy) : 
	AttackingBehavior(enemy),
	EnemyAttackingBehavior(enemy) {
}

sf::Color NeutralBehavior::getConfiguredHealthColor() const {
	return COLOR_NEUTRAL;
}

void NeutralBehavior::updateAggro() {
	// TODO: check for stealing cendric.
}