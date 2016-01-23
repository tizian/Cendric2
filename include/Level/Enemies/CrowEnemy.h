#pragma once

#include "global.h"
#include "Level/FlyingEnemy.h"
#include "Level/Level.h"
#include "SpellManager.h"
#include "Screen.h"

// A flying crow
class CrowEnemy : public Enemy {
public:
	CrowEnemy(Level* level, Screen* screen);
	~CrowEnemy() {}

	void loadAnimation() override;

	MovingBehavior* createMovingBehavior() override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;

	sf::Time getConfiguredWaitingTime() const override;
	sf::Time getConfiguredChasingTime() const override;

	sf::Vector2f getConfiguredSpellOffset() const override;

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override;
	
protected:
	void handleAttackInput();
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	// loads spells and adds them to the spell manager. default does nothing.
	void loadSpells() override;
};