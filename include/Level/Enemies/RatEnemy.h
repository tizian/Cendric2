#pragma once

#include "global.h"
#include "Level/Enemy.h"
#include "Level/Level.h"
#include "SpellManager.h"
#include "Screen.h"

// A nasty rat in a level
class RatEnemy : public Enemy {
public:
	RatEnemy(Level* level, Screen* screen);
	~RatEnemy() {}

	float getMaxVelocityYUp() const override;
	float getMaxVelocityYDown() const override;
	float getMaxVelocityX() const override;

	MovingBehavior* createMovingBehavior() override;
	AttackingBehavior* createAttackingBehavior() override;

	bool getFleeCondition() const override;

	sf::Time getConfiguredFightAnimationTime() const override;
	sf::Vector2f getConfiguredSpellOffset() const override;

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override;
	
protected:
	void handleAttackInput();
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	// loads spells and adds them to the spell manager. default does nothing.
	void loadSpells() override;
	void loadAnimation() override;
};
