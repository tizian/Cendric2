#pragma once

#include "global.h"
#include "Level/Enemy.h"

class Level;
class Screen;

// skins:
// 0 : default (ice cairn)
// 1 : fire cairn
class CairnEnemy final : public Enemy {
public:
	CairnEnemy(const Level* level, Screen* screen);
	~CairnEnemy() {}

	int getMentalStrength() const override;

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override;
	void insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const override;

	EnemyID getEnemyID() const override { return EnemyID::Cairn; }

protected:
	std::string getSpritePath() const override;
	std::string getDeathSoundPath() const override;
	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;
	void handleAttackInput();
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	// loads spells and adds them to the spell manager. default does nothing.
	void loadSpells() override;
	void loadAnimation(int skinNr) override;
};