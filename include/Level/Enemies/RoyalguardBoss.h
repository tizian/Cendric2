#pragma once

#include "global.h"
#include "Level/Enemy.h"
#include "GameObjectComponents/ParticleComponent.h"

enum class WeaponRotateType {
	Fixed,
	ToMainChar,
	Turn
};

enum class RoyalguardBossState {
	Waiting,
	ChargingStart,
	Charging,
	Swirl,
	FireballStart,
	BottomFire,
	TopFire,
	Healing,
	Dead,
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
	RoyalguardBossState getBossState() const{ return m_bossState; }
	void clearBossState();
	
	void notifyOtherDeath(const sf::Vector2f& newPos, RoyalguardBoss* other);
	void revive();

protected: 
	void loadBoss();
	void loadWeapon();
	void updateWeapon(const sf::Time& frameTime);
	virtual std::string getWeaponTexturePath() const = 0;
	virtual void updateBossState(const sf::Time& frameTime) = 0;
	virtual void loadParticles() = 0;
	void loadHealingParticles();

protected:
	sf::Sprite m_weapon;
	sf::Vector2f m_weaponOffset;
	bool m_isWeaponVisible = false;
	bool m_isOtherDead = false;
	bool m_isTopFire = false;
	RoyalguardBoss* m_other;
	WeaponRotateType m_weaponRotateType = WeaponRotateType::Fixed;
	RoyalguardBossState m_bossState = RoyalguardBossState::Waiting;
	sf::Time m_stateTime;
	ParticleComponent* m_healingPc;

	std::vector<ParticleComponent*> m_pcs;

	static const sf::Vector2f WEAPON_ORIGIN;
	static const sf::Time HEALING_TIME;
};