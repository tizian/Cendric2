#pragma once

#include "global.h"
#include "Level/Enemy.h"

enum class WeaponRotateType {
	Fixed,
	ToMainChar,
	Turn
};

class RoyalguardBoss : public virtual Enemy {
public:
	RoyalguardBoss(const Level* level, Screen* screen);
	virtual ~RoyalguardBoss() {};

	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;
	void setDead() override;

	sf::Time getConfiguredWaitingTime() const override;
	int getMentalStrength() const override { return 4; };
	float getConfiguredDistanceToHPBar() const override;
	
	void notifyOtherDeath(const sf::Vector2f& newPos);

protected: 
	void loadWeapon();
	void updateWeapon(const sf::Time& frameTime);
	virtual std::string getWeaponTexturePath() const = 0;

protected:
	sf::Sprite m_weapon;
	bool m_isWeaponVisible = false;
	bool m_isOtherDead = false;
	WeaponRotateType m_weaponRotateType = WeaponRotateType::Fixed;

	static const sf::Vector2f WEAPON_ORIGIN;
};