#pragma once

#include "global.h"

#include "Level/Enemy.h"

// An enemy that walks and jumps
class WalkingEnemy : virtual public Enemy {
public:
	WalkingEnemy(Level* level, LevelMainCharacter* mainChar, EnemyID id);
	virtual ~WalkingEnemy();

	void checkCollisions(const sf::Vector2f& nextPosition) override;

protected:
	virtual void handleMovementInput() override;
	// a decision the enemy has taken an that lasts until it decides anew. 0 means wait, -1 go left, 1 go right
	int m_randomDecision = 0;
	virtual void makeRandomDecision() override;
	
	// if this bool is set to true, the enemy jumps in the next frame. used by the AI
	bool m_jumps = false;

	float m_jumpHeight = 0;
	virtual float getDistanceToAbyss() const;

private:
	
};