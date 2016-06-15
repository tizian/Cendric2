#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/EnemyMovingBehavior.h"

#include "Enums/LevelDynamicTileID.h"

// A moving behavior for flying enemies.
class FlyingBehavior : public virtual EnemyMovingBehavior {
public:
	FlyingBehavior(Enemy* enemy);
	~FlyingBehavior() {};

	void checkCollisions(const sf::Vector2f& nextPosition) override;
	void updateAnimation() override;
	void makeRandomDecision() override;
	void handleDefaultAcceleration() override;
};