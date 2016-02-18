#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/EnemyMovingBehavior.h"

#include "Enums/LevelDynamicTileID.h"

// A moving behavior for flying enemies.
class FlyingBehavior : public virtual EnemyMovingBehavior {
public:
	FlyingBehavior(Enemy* enemy);
	~FlyingBehavior() {};

	void checkCollisions(const sf::Vector2f& nextPosition) override;
	void updateAnimation() override;
	void makeRandomDecision() override;

protected:
	// a flying enemy will never fly into those and handle them as collidable tiles.
	// default is water, spikes top, spikes bottom.
	std::set<LevelDynamicTileID> m_avoidableTiles;

	int m_randomDecisionX = 0;
	// 0 means wait, -1 go up, 1 go down
	int m_randomDecisionY = 0;
};