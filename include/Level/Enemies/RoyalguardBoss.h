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
	void loadWeapon();
	virtual std::string getWeaponTexturePath() const = 0;

protected:
	sf::Sprite m_weapon;
	bool m_isOtherDead = false;
};