#pragma once

#include "global.h"
#include "Level/MOBBehavior/AttackingBehaviors/EnemyAttackingBehavior.h"

// An aggressive enemy attacking behavior. These enemies attack cendric
class AggressiveBehavior final : public EnemyAttackingBehavior {
public:
	AggressiveBehavior(Enemy* enemy);

	void updateAggro() override;
	sf::Color getConfiguredHealthColor() const override;
};