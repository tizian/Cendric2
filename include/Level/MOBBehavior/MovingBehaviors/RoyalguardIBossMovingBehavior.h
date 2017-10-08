#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/WalkingBehavior.h"

class RoyalguardBoss;

// A moving behavior for the mob "RoyalguardIBoss"
class RoyalguardIBossMovingBehavior final : public WalkingBehavior {
public:
	RoyalguardIBossMovingBehavior(RoyalguardBoss* enemy);
	~RoyalguardIBossMovingBehavior() {};

	void execHandleMovementInput() override;
	void updateAnimation(const sf::Time& frameTime) override;

private:
	RoyalguardBoss* m_boss;
};