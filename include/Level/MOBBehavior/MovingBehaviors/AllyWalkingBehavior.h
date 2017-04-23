#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/WalkingBehavior.h"

// A moving behavior for allied walking enemies.
class AllyWalkingBehavior final : public WalkingBehavior {
public:
	AllyWalkingBehavior(Enemy* enemy);

	void checkCollisions(const sf::Vector2f& nextPosition) override;
	void setReplaceDistance(float replaceDistance);

	void execHandleMovementInput() override;

private:
	float m_replaceDistance;
};