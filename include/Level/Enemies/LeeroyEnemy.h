#pragma once

#include "global.h"
#include "Level/Enemy.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"

class LeeroyEnemy final : public Enemy {
public:
	LeeroyEnemy(const Level* level, Screen* screen);

	int getMentalStrength() const override;

	float getConfiguredDistanceToHPBar() const override;

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override {}
	void insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const override {}

	EnemyID getEnemyID() const override { return EnemyID::Leeroy; }

protected:
	std::string getSpritePath() const override;
	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;
	void handleAttackInput();
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	// loads spells and adds them to the spell manager. default does nothing.
	void loadSpells() override;
	void loadAnimation(int skinNr) override;
};