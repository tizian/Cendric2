#pragma once

#include "global.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screens/Screen.h"

// A hatching dragon whelp
// 0, red whelp, egg.
// 1, red whelp, hatched.
// 2, purple whelp, egg.
// 3, purple whelp, hatched

enum class DragonWhelpState {
	Egg,
	Hatching,
	Active
};

class DragonWhelpEnemy final : public Enemy {
public:
	DragonWhelpEnemy(const Level* level, Screen* screen);

	void update(const sf::Time& frameTime) override;

	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;

	sf::Time getConfiguredWaitingTime() const override;
	sf::Time getConfiguredChasingTime() const override;

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override;
	void insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const override;

	float getConfiguredDistanceToHPBar() const override;
	EnemyID getEnemyID() const override { return EnemyID::Dragonwhelp; }
	void onHit(Spell* spell) override;
	void setDead() override;
	
protected:
	std::string getSpritePath() const override;
	std::string getDeathSoundPath() const override;
	void handleAttackInput();
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	// loads spells and adds them to the spell manager. default does nothing.
	void loadSpells() override;
	void loadAnimation(int skinNr) override;

	DragonWhelpState m_dragonState;
	void setHatching();
	void setActive();
	void setInactive();

	sf::Time m_hatchingTime = sf::Time::Zero;
};