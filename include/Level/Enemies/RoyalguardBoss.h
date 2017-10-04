#pragma once

#include "global.h"
#include "Level/Enemy.h"

class RoyalguardBoss : public virtual Enemy {
public:
	RoyalguardBoss(const Level* level, Screen* screen);
	virtual ~RoyalguardBoss() {};

	void setDead() override;

	sf::Time getConfiguredWaitingTime() const override;
	int getMentalStrength() const override { return 4; };
	float getConfiguredDistanceToHPBar() const override;
	
	void notifyOtherDeath(const sf::Vector2f& newPos);

protected:

	bool m_isOtherDead = false;
};