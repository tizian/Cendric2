#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/FlyingBehavior.h"

// A moving behavior for the mob "ElysiaBoss"
class ElysiaBossMovingBehavior : public virtual FlyingBehavior {
public:
	ElysiaBossMovingBehavior(Enemy* enemy);

	void update(const sf::Time& frameTime) override;
	void execHandleMovementInput() override;
};