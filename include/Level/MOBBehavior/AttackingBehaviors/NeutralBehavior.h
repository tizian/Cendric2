#pragma once

#include "global.h"
#include "Level/MOBBehavior/AttackingBehaviors/EnemyAttackingBehavior.h"

// A neutral enemy attacking behavior. These enemies leave cendric alone and cannot be attacked
class NeutralBehavior final : public EnemyAttackingBehavior {
public:
	NeutralBehavior(Enemy* enemy);
	~NeutralBehavior() {};

	void updateAggro() override;

	sf::Color getConfiguredHealthColor() const override;
};