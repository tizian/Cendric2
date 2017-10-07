#pragma once

#include "global.h"
#include "Level/Enemies/RoyalguardBoss.h"

class RoyalguardIBoss final : public virtual RoyalguardBoss {
public:
	RoyalguardIBoss(const Level* level, Screen* screen);

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
};

class RoyalguardFire final : public GameObject {
	RoyalguardFire(bool isTop, LevelMovableGameObject* mainChar);

	void update(const sf::Time& frameTime) override;

	GameObjectType getConfiguredType() const override { return GameObjectType::_Spell; }

private:
	LevelMovableGameObject* m_mainChar;
	void loadParticles();

	static const sf::Time GRACE_TIME;
	sf::Time m_graceTime;
};