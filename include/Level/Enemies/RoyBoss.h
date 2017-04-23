#pragma once

#include "global.h"
#include "Level/Enemy.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"

class RoyBoss final : public Enemy {
public:
	RoyBoss(const Level* level, Screen* screen);

	void setDead() override;

	sf::Time getConfiguredWaitingTime() const override;
	EnemyID getEnemyID() const override { return EnemyID::Boss_Roy; }

	int getMentalStrength() const override { return 4; };
	float getConfiguredDistanceToHPBar() const override;

	void notifyJeremyDeath(const sf::Vector2f& newPos);
	void notifyMorgianaDeath(const sf::Vector2f& newPos);

protected:
	std::string getSpritePath() const override;
	std::string getDeathSoundPath() const override;
	
	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;
	void handleAttackInput();
	void loadAttributes() override;
	void loadSpells() override;
	void loadAnimation(int skinNr) override;
};