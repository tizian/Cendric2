#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/EnemyMovingBehavior.h"

enum class CreepingDirection {
	Bottom,
	Top,
	Left,
	Right
};

// A moving behavior for creeping enemies.
class CreepingBehavior : public virtual EnemyMovingBehavior {
public:
	CreepingBehavior(Enemy* enemy);
	virtual ~CreepingBehavior() {};

	void checkCollisions(const sf::Vector2f& nextPosition) override;
	void makeRandomDecision() override;
	void updateAnimation(const sf::Time& frameTime) override;

	void flipGravity() override {};
	bool isUpsideDown() const override { return false; };

protected:
	void handleDefaultAcceleration() override;
	void handleTrueAcceleration() override;

	void checkCreepingXYDirection(const sf::Vector2f& nextPosition, bool& collidesX, bool& collidesY);
	bool isCreepingOnX() const;
	bool isFallingDown(bool isMovingDown, bool isMovingRight) const;
	CreepingDirection m_creepingDirection;

	void setCreepingDirection(CreepingDirection dir);
	sf::Vector2f m_horizontalSpriteOffset;
	sf::Vector2f m_verticalSpriteOffset;
};