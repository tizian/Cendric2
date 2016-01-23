#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehavior.h"
#include "Level/Enemy.h"

// A moving behavior for enemies.
class EnemyMovingBehavior : public virtual MovingBehavior {
public:
	EnemyMovingBehavior(Enemy* enemy);
	virtual ~EnemyMovingBehavior() {};

	virtual void makeRandomDecision() = 0;
	// the distance from the center of the enemy to the center of the main char at which the enemy approaches its target.
	void setApproachingDistance(float distance);

protected:
	Enemy* m_enemy;
	int m_randomDecision = 0;
	float m_approachingDistance = 0.f;
};