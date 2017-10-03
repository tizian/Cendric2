#pragma once

#include "global.h"
#include "Level/Enemy.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"

class RoyalguardIBoss final : public Enemy {
public:
	RoyalguardIBoss(const Level* level, Screen* screen);

	void update(const sf::Time& frameTime) override;
	void setDead() override;

	sf::Time getConfiguredWaitingTime() const override;
	EnemyID getEnemyID() const override { return EnemyID::Boss_Royalguard_I; }

	int getMentalStrength() const override { return 4; };
	float getConfiguredDistanceToHPBar() const override;
	
	void notifyOtherDeath(const sf::Vector2f& newPos);

protected:
	std::string getSpritePath() const override;
	std::string getDeathSoundPath() const override;
	
	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;
	void handleAttackInput();
	void loadAttributes() override;
	void loadSpells() override;
	void loadAnimation(int skinNr) override;

	bool m_isOtherDead = false;
};