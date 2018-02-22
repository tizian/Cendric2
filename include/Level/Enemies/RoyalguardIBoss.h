#pragma once

#include "global.h"
#include "Level/Enemies/RoyalguardBoss.h"
#include "Structs/SpellData.h"

class LightObject;

namespace particles {
	class LineSpawner;
	class AngledVelocityGenerator;
}

class RoyalguardIBoss final : public RoyalguardBoss {
public:
	RoyalguardIBoss(const Level* level, Screen* screen);

	void addDamage(int damage, DamageType damageType, bool overTime, bool critical) override;
	EnemyID getEnemyID() const override { return EnemyID::Boss_Royalguard_I; }

protected:
	void updateBossState(const sf::Time& frameTime) override;

	std::string getSpritePath() const override;
	std::string getDeathSoundPath() const override;
	std::string getWeaponTexturePath() const override;
	
	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;
	void handleAttackInput();
	void loadAttributes() override;
	void loadSpells() override;
	void loadAnimation(int skinNr) override;
	void loadParticles() override;
	void loadFireParticles();
	void loadFireLineParticles();

private:
	ParticleComponent* m_firePc;
	ParticleComponent* m_fireLinePc;
	particles::LineSpawner* m_lineSpawner;
	particles::AngledVelocityGenerator* m_lineVelGen;

protected:
	sf::Time m_fireTime;

public:
	static const sf::Time FIRE_TIME;
};

class RoyalguardFire final : public GameObject {
public:
	RoyalguardFire(bool isTop, LevelMovableGameObject* mainChar);

	void update(const sf::Time& frameTime) override;

	GameObjectType getConfiguredType() const override { return _Spell; }

private:
	LevelMovableGameObject* m_mainChar;
	void loadParticles();

	static const sf::Time GRACE_TIME;
	static const int FIRE_DAMAGE;
	static const std::string FIRE_SOUND;

	sf::Time m_graceTime;
	sf::Time m_timeSinceHurt;

	bool m_isTop;
	particles::BoxSpawner* m_posGen;
	LightObject* m_lightObject = nullptr;
	sf::Time m_ttl;

public:
	static const sf::Vector2f FIRE_POS_TOP;
	static const sf::Vector2f FIRE_POS_BOT;
	static const sf::Vector2f FIRE_EXTENTS;
};