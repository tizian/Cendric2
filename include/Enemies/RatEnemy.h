#pragma once

#include "global.h"
#include "Level/Enemy.h"
#include "Level/Level.h"
#include "SpellManager.h"
#include "Screen.h"

// A nasty rat in a level
class RatEnemy : public Enemy {
public:
	RatEnemy(Level* level, LevelMainCharacter* mainChar);
	virtual ~RatEnemy() {}

	void load() override;

	float getConfiguredMaxVelocityYUp() const override;
	float getConfiguredMaxVelocityYDown() const override;
	float getConfiguredMaxVelocityX() const override;
	float getApproachingDistance() const override;
	float getAggroRange() const override;
	bool getFleeCondition() const override;
	float getDistanceToAbyss() const override;
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