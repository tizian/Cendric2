#pragma once

#include "global.h"
#include "Enemy.h"
#include "Level.h"
#include "SpellManager.h"
#include "Screen.h"

// A nasty rat in a level
class RatEnemy : public Enemy
{
public:
	RatEnemy(Level* level, LevelMainCharacter* mainChar);
    virtual ~RatEnemy() {}

	void load() override;

	float getConfiguredMaxVelocityY() const override;
	float getConfiguredMaxVelocityX() const override;
	float getConfiguredApproachingDistance() const override;
	float getConfiguredAggroRange() const override;
	bool getConfiguredFleeCondition() const override;
	sf::Time getConfiguredFightAnimationTime() const override;
	sf::Vector2f getConfiguredSpellOffset() const override;

protected:
	// handle input and calculate the next position
	void handleMovementInput() override;
	void handleAttackInput() override;
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	// loads spells and adds them to the spell manager. default does nothing.
	void loadSpells() override;
};