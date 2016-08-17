#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/EnemyMovingBehavior.h"

// A moving behavior for observer enemies.
class ObserverBehavior : public virtual EnemyMovingBehavior {
public:
	ObserverBehavior(Enemy* enemy);
	~ObserverBehavior() {};

	void execHandleMovementInput() override;
	void checkCollisions(const sf::Vector2f& nextPosition) override;
	void updateAnimation() override;
	void makeRandomDecision() override;
	void handleDefaultAcceleration() override;
};