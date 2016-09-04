#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/EnemyMovingBehavior.h"
#include "Structs/AIWalkingQueryRecord.h"

// A moving behavior for walking enemies.
class WalkingBehavior : public virtual EnemyMovingBehavior {
public:
	WalkingBehavior(Enemy* enemy);
	virtual ~WalkingBehavior() {};

	void checkCollisions(const sf::Vector2f& nextPosition) override;
	void makeRandomDecision() override;
	void updateAnimation(const sf::Time& frameTime) override;
	void calculateJumpHeight();

	void setDistanceToAbyss(float distance);
	void setIgnoreDynamicTiles(bool value) override;

	float getDistanceToAbyss() const;

protected:
	AIWalkingQueryRecord m_aiRecord;

	// this function returns whether an ai jump has been calculated.
	// check the ai Record for further details if this function returns true.
	// the argument decides whether only a jump should be checked or if the option to walk on 
	// should also be considered.
	bool doAIJump(bool onlyJump);

	bool m_jumps = false;
	bool m_walksBlindly = false;
	bool m_collidesX = false;
};