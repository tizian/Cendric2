#pragma once

#include "global.h"
#include <set>
#include "Enums/LevelDynamicTileID.h"

#include "Level/Enemy.h"

// An enemy that is flying
class FlyingEnemy : virtual public Enemy {
public:
	FlyingEnemy(Level* level, Screen* screen, bool isControlled = false);
	virtual ~FlyingEnemy();

	void checkCollisions(const sf::Vector2f& nextPosition) override;

protected:
	virtual void handleMovementInput() override;
	// a decision the enemy has taken an that lasts until it decides anew. 0 means wait, -1 go left, 1 go right
	int m_randomDecisionX = 0;
	// 0 means wait, -1 go up, 1 go down
	int m_randomDecisionY = 0;
	virtual void makeRandomDecision() override;

	void updateAnimation(const sf::Time& frameTime) override;

	// a flying enemy will never fly into those and handle them as collidable tiles.
	// default is water, spikes top, spikes bottom.
	std::set<LevelDynamicTileID> m_avoidableTiles;
private:

};