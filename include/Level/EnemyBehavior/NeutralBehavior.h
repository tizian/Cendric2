#pragma once

#include "global.h"
#include "Level/EnemyBehavior/EnemyAttackingBehavior.h"

// A neutral enemy attacking behavior. These enemies leave cendric alone but switch to 
// aggressive behavior if he steals something (chest, item) and is in aggro range
class NeutralBehavior : public EnemyAttackingBehavior {
public:
	NeutralBehavior(Enemy* enemy);
	~NeutralBehavior() {};

	void updateAggro() override;

	EnemyAttitude getAttitude() const override;
	sf::Color getConfiguredHealthColor() const override;
};