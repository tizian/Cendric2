#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/WalkingBehavior.h"

// A moving behavior for the mob "WolfBoss"
class WolfBossMovingBehavior : public virtual WalkingBehavior {
public:
	WolfBossMovingBehavior(Enemy* enemy);
	~WolfBossMovingBehavior();

	void update(const sf::Time& frameTime) override;
	void execHandleMovementInput() override;
	void checkCollisions(const sf::Vector2f& nextPosition) override;
	void updateAnimation() override;
	void setFacingRight(bool value) override;

private:
	sf::Time m_tripOverTime = sf::Time::Zero;
	sf::Time m_layingTime = sf::Time::Zero;
	sf::Time m_standupTime = sf::Time::Zero;

	int m_chargeMovingDirection = 1;
	sf::Sound m_sound;
};