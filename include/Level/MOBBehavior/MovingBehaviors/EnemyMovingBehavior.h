#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehavior.h"
#include "Level/Enemy.h"

// A moving behavior for enemies.
class EnemyMovingBehavior : public virtual MovingBehavior {
public:
	EnemyMovingBehavior(Enemy* enemy);
	virtual ~EnemyMovingBehavior();

	virtual void makeRandomDecision() = 0;
	// the distance from the center of the enemy to the center of the main char at which the enemy approaches its target.
	void setApproachingDistance(float distance);
	void handleMovementInput() override;
	void setFacingRight(bool value) override;

	// sets the target where an enemy wants to go to. As long as this is not reset, 
	// the enemy will ignore its current target and try to move to this spot.
	void setMovingTarget(int x, int y);
	void resetMovingTarget();

protected:
	Enemy* m_enemy;
	float m_approachingDistance = 0.f;

	sf::Vector2f* m_movingTarget = nullptr;

	// 0 means stand still, 1 move right, -1 move left
	int m_movingDirectionX = 0;
	// 0 means stand still, 1 move down, -1 move up
	int m_movingDirectionY = 0;

	virtual void execHandleMovementInput() = 0;
	virtual void handleTrueAcceleration() = 0;
};