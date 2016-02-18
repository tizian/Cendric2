#pragma once

#include "global.h"
#include "Level/MOBBehavior/AttackingBehaviors/EnemyAttackingBehavior.h"

// An aggressive enemy attacking behavior. These enemies attack cendric
class AggressiveBehavior : public EnemyAttackingBehavior {
public:
	AggressiveBehavior(Enemy* enemy);
	~AggressiveBehavior() {};

	void updateAggro() override;

	sf::Color getConfiguredHealthColor() const override;
};