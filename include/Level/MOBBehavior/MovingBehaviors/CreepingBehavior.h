#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/EnemyMovingBehavior.h"

enum class CreepingDirection {
	Bottom,
	Top,
	Left,
	Right
};

// A moving behavior for creeping enemies.
class CreepingBehavior : public virtual EnemyMovingBehavior {
public:
	CreepingBehavior(Enemy* enemy);
	virtual ~CreepingBehavior() {};

	void checkCollisions(const sf::Vector2f& nextPosition) override;
	void makeRandomDecision() override;
	void updateAnimation(const sf::Time& frameTime) override;

protected:
	bool m_collidesX = false;
	CreepingDirection m_creepingDirection;
};