#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/FlyingBehavior.h"

class YashaBoss;

// A moving behavior for the mob "YashaBoss"
class YashaBossMovingBehavior final : public FlyingBehavior {
public:
	YashaBossMovingBehavior(YashaBoss* enemy);
	~YashaBossMovingBehavior();

	void update(const sf::Time& frameTime) override;
	void execHandleMovementInput() override;
	void updateAnimation(const sf::Time& frameTime) override;

private:
	YashaBoss* m_boss;
};