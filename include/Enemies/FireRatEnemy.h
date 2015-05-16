#pragma once

#include "global.h"
#include "Enemy.h"
#include "Level.h"
#include "SpellManager.h"
#include "Screen.h"

// A nasty fiery rat in a level
class FireRatEnemy : public Enemy
{
public:
	FireRatEnemy(Level* level, LevelMainCharacter* mainChar);
	~FireRatEnemy();

	void load() override;
	void checkCollisions(const sf::Vector2f& nextPosition) override;

	float getConfiguredMaxVelocityY() const override;
	float getConfiguredMaxVelocityX() const override;
	sf::Vector2f getConfiguredSpellOffset() const override;

protected:
	// handle input and calculate the next position
	void handleInput() override;
};