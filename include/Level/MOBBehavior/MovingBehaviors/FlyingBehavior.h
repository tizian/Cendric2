#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/EnemyMovingBehavior.h"

#include "Enums/LevelDynamicTileID.h"

// A moving behavior for flying enemies.
class FlyingBehavior : public virtual EnemyMovingBehavior {
public:
	FlyingBehavior(Enemy* enemy);
	virtual ~FlyingBehavior() {};

	void checkCollisions(const sf::Vector2f& nextPosition) override;
	void updateAnimation(const sf::Time& frameTime) override;
	void makeRandomDecision() override;

protected:
	void handleDefaultAcceleration() override;
	void handleTrueAcceleration() override;
};