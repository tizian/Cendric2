#pragma once

#include "global.h"
#include "Level/Enemy.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"

// Oozes can creep on walls...
// skins:
// 1 : shadowy green slime
// 2 : lava slime
class OozeEnemy final : public Enemy {
public:
	OozeEnemy(const Level* level, Screen* screen);

	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override;
	void insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const override;

	EnemyID getEnemyID() const override { return EnemyID::Ooze; }
	int getMentalStrength() const override { return 4; }
	sf::Time getConfiguredRandomDecisionTime() const override; 
	sf::Time getConfiguredChasingTime() const override;
	sf::Time getConfiguredWaitingTime() const override;

protected:
	std::string getSpritePath() const override;
	std::string getDeathSoundPath() const override;
	void handleAttackInput();
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	// loads spells and adds them to the spell manager. default does nothing.
	void loadSpells() override;
	void loadAnimation(int skinNr) override;
};