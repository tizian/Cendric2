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
	void updateAnimation() override;
	void calculateJumpHeight();

	void setDistanceToAbyss(float distance);
	void setIgnoreDynamicTiles(bool value) override;

	float getDistanceToAbyss() const;

	int getRandomDecision() const;

protected:
	AIWalkingQueryRecord m_aiRecord;

	// this function returns whether an 
	// ai jump should be done. check the ai Record for further details if this function returns true.
	bool doAIJump();

	float m_jumpHeight;
	bool m_jumps = false;
	bool m_walksBlindly = false;
	bool m_collidesX = false;
	int m_randomDecision = 0;
};