#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/WalkingBehavior.h"

// A moving behavior for allied walking enemies.
class AllyWalkingBehavior : public virtual WalkingBehavior {
public:
	AllyWalkingBehavior(Enemy* enemy);

	void checkCollisions(const sf::Vector2f& nextPosition) override;

	void execHandleMovementInput() override;
};