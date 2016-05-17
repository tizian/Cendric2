#pragma once

#include "global.h"
#include "Level/Enemy.h"

class Level;
class Screen;

class StonemanEnemy : public Enemy {
public:
	StonemanEnemy(const Level* level, Screen* screen);
	~StonemanEnemy() {}

	void loadAnimation() override;

	sf::Vector2f getConfiguredSpellOffset() const override;
	int getMentalStrength() const override;

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override;

protected:
	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;
	void handleAttackInput();
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	// loads spells and adds them to the spell manager. default does nothing.
	void loadSpells() override;
};