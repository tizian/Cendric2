#pragma once

#include "global.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"

// skins:
// 0: thief
// 1: necro
// 2: cleric
class VeliusBossClone final : public Enemy {
public:
	VeliusBossClone(const Level* level, Screen* screen);

	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;

	void update(const sf::Time& frameTime) override;
	void setDead() override;

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override {};
	void insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const override {};

	EnemyID getEnemyID() const override { return EnemyID::VeliusClone; }
	int getMentalStrength() const override { return 4; }

private:
	std::string getSpritePath() const override;
	std::string getDeathSoundPath() const override;
	void handleAttackInput();
	void loadAttributes() override;

	void loadSpells() override;
	void loadAnimation(int skinNr) override;
};