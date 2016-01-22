#pragma once

#include "global.h"
#include "Level/EnemyBehavior/MovingBehavior.h"

// A moving behavior for walking enemies.
class WalkingBehavior : public MovingBehavior {
public:
	WalkingBehavior(Enemy* enemy) : MovingBehavior(enemy) {};
	~WalkingBehavior() {};

	void checkCollisions(const sf::Vector2f& nextPosition) override;
	void handleMovementInput() override;
	void makeRandomDecision() override;
	void updateAnimation() override;

	void setJumpHeight(float height);

	float getJumpHeight() const;

	void setDistanceToAbyss(float distance);

	float getDistanceToAbyss() const;

	int getRandomDecision() const;

private:
	bool m_jumps = false;
	float m_distanceToAbyss = 0.f;
	float m_jumpHeight = 0.f;
	int m_randomDecision = 0;
};