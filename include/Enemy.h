#pragma once

#include "global.h"
#include "LevelMovableGameObject.h"
#include "Enums/EnemyID.h"

class Level;
class LevelMainCharacter;

// An enemy in a level
class Enemy : public LevelMovableGameObject
{
public:
	Enemy(Level* level, LevelMainCharacter* mainChar);

	void checkCollisions(const sf::Vector2f& nextPosition) override;

	GameObjectType getConfiguredType() const override;
	sf::Color getConfiguredDebugColor() const override;

protected:
	// the target to be destroyed!
	LevelMainCharacter* m_mainChar;
	float distToMainChar() const;
	// if this bool is set to true, the enemy jumps in the next frame. used by the AI
	bool m_jumps = false;
};