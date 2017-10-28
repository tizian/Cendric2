#pragma once

#include "global.h"
#include "Level/Enemy.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"

class KingEnemy final : public Enemy {
public:
	KingEnemy(const Level* level, Screen* screen);

	int getMentalStrength() const override;

	float getConfiguredDistanceToHPBar() const override;
	sf::Time getConfiguredWaitingTime() const override { return sf::Time::Zero; }

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override {}
	void insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const override {}

	EnemyID getEnemyID() const override { return EnemyID::King; }

protected:
	std::string getSpritePath() const override;
	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;

	void handleAttackInput();
	void loadAttributes() override;
	void loadSpells() override;
	void loadAnimation(int skinNr) override;
};