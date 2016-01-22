#pragma once

#include "global.h"
#include "Level/Enemy.h"

// An enemy moving behavior
class MovingBehavior {
public:
	MovingBehavior(Enemy* enemy);
	virtual ~MovingBehavior() {};

	virtual void update(const sf::Time& frameTime);
	virtual void updateAnimation() = 0;

	virtual void checkCollisions(const sf::Vector2f& nextPosition) = 0;

	virtual void handleMovementInput() = 0;

	virtual void makeRandomDecision() = 0;

	void setApproachingDistance(float distance);

	// the distance from the center of the enemy to the center of the main char at which the enemy approaches its target.
	float getApproachingDistance() const;

	

protected:
	Enemy* m_enemy;
	float m_approachingDistance = 0.f;
};