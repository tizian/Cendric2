#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/WalkingBehavior.h"

class RoyalguardBoss;

// A moving behavior for the mob "RoyalguardTBoss"
class RoyalguardTBossMovingBehavior final : public WalkingBehavior {
public:
	RoyalguardTBossMovingBehavior(RoyalguardBoss* enemy);
	~RoyalguardTBossMovingBehavior() {};

	void execHandleMovementInput() override;
	void checkCollisions(const sf::Vector2f& nextPosition) override;
	void updateAnimation(const sf::Time& frameTime) override;
	void setChargingDirection(const sf::Vector2f& direction);

private:
	sf::Vector2f m_chargingDirection;
	RoyalguardBoss* m_boss;

	static const float CHARGING_SPEED;
};