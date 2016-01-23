#pragma once

#include "global.h"
#include "Level/EnemyBehavior/EnemyMovingBehavior.h"

// A moving behavior for walking enemies.
class WalkingBehavior : virtual public EnemyMovingBehavior {
public:
	WalkingBehavior(Enemy* enemy);
	virtual ~WalkingBehavior() {};

	void checkCollisions(const sf::Vector2f& nextPosition) override;
	void handleMovementInput() override;
	void makeRandomDecision() override;
	void updateAnimation() override;
	void calculateJumpHeight();


	void setDistanceToAbyss(float distance);

	float getDistanceToAbyss() const;

	int getRandomDecision() const;

protected:
	bool m_jumps = false;
	float m_distanceToAbyss = 0.f;
	float m_jumpHeight = 0.f;
	int m_randomDecision = 0;
};