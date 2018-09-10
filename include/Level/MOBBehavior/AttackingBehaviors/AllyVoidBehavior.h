#pragma once

#include "global.h"
#include "Level/MOBBehavior/AttackingBehaviors/EnemyAttackingBehavior.h"

// An ally that cannot attack at all
class AllyVoidBehavior final : public EnemyAttackingBehavior {
public:
	AllyVoidBehavior(Enemy* enemy);
	~AllyVoidBehavior() {};
	
	sf::Color getConfiguredHealthColor() const override;

	void updateAggro() override;
};