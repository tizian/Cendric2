#pragma once

#include "global.h"
#include "Level/EnemyBehavior/AttackingBehavior.h"

// A neutral enemy attacking behavior. These enemies leave cendric alone but switch to 
// aggressive behavior if he steals something (chest, item) and is in aggro range
class NeutralBehavior : public AttackingBehavior {
public:
	NeutralBehavior(Enemy* enemy) : AttackingBehavior(enemy) {};
	~NeutralBehavior() {};

	void updateAggro() override;

	EnemyAttitude getAttitude() const override;
	sf::Color getConfiguredHealthColor() const override;
};