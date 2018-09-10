#include "Level/MOBBehavior/AttackingBehaviors/AllyVoidBehavior.h"

AllyVoidBehavior::AllyVoidBehavior(Enemy* enemy) :
	AttackingBehavior(enemy),
	EnemyAttackingBehavior(enemy) {
}

sf::Color AllyVoidBehavior::getConfiguredHealthColor() const {
	return COLOR_GOOD;
}

void AllyVoidBehavior::updateAggro() {
	// nop
}