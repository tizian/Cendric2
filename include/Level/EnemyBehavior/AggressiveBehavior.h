#pragma once

#include "global.h"
#include "Level/EnemyBehavior/AttackingBehavior.h"

// An aggressive enemy attacking behavior. These enemies attack cendric
class AggressiveBehavior : public AttackingBehavior {
public:
	AggressiveBehavior(Enemy* enemy) : AttackingBehavior(enemy) {};
	~AggressiveBehavior() {};

	void updateAggro() override;

	EnemyAttitude getAttitude() const override;
	sf::Color getConfiguredHealthColor() const override;
};