#pragma once

#include "global.h"
#include "Level/LevelMovableGameObject.h"
#include "Level/LevelMainCharacter.h"

// An enemy moving behavior
class MovingBehavior {
public:
	MovingBehavior(LevelMovableGameObject* mob);
	virtual ~MovingBehavior() {};

	virtual void update(const sf::Time& frameTime);
	// update animation based on the current velocity + grounded
	virtual void updateAnimation() = 0;

	virtual void checkCollisions(const sf::Vector2f& nextPosition) = 0;
	
	// the mob ignores collidable dynamic tiles in its collision logic but still collides with strictly dynamic tiles
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const;

	virtual void handleMovementInput() = 0;

	// gravity flip (used for anti gravity spell)
	void flipGravity();
	// change gravity (used for leap of faith spell)
	void setGravityScale(float scale);
	// change x speed (used for ghost spell)
	void setMaxXVelocityScale(float scale);
	// the mob ignores collidable dynamic tiles in its collision logic but still collides with strictly dynamic tiles
	virtual void setIgnoreDynamicTiles(bool value);

	void setMaxVelocityX(float vel);
	void setMaxVelocityYUp(float vel);
	void setMaxVelocityYDown(float vel);

	float getMaxVelocityX() const;
	float getMaxVelocityYUp() const;
	float getMaxVelocityYDown() const;

	void setWalkAcceleration(float acceleration);
	void setGravityAcceleration(float acceleration);
	// choose a value between 0.9 for really slow halting and 1.0f for aprupt halting.
	void setDampingGroundPerS(float damping);
	void setDampingAirPerS(float damping);
	void setFightAnimation(const sf::Time& animationTime, GameObjectState animation, bool isBlocking);
	float getGravity() const;
	bool isGrounded() const;
	bool isFacingRight() const;
	bool isUpsideDown() const;
	bool isIgnoreDynamicTiles() const;
	
protected:
	// helper method used for collision checking
	void checkXYDirection(const sf::Vector2f& nextPosition, bool& collidesX, bool& collidesY);
	// if the position delta is too high (> tileSize), the mob is killed and no further collisions are checked.
	void checkForCollisionTilt(const sf::Vector2f& oldPosition);
	bool m_isCollisionTilt = false;
	bool m_isCollisionTiltSuppressed = false;

	LevelMovableGameObject* m_mob;
	LevelMainCharacter* m_mainChar;
	bool m_ignoreDynamicTiles = false;
	bool m_isFacingRight = true;
	bool m_nextIsFacingRight = true;
	bool m_isGrounded = false;
	bool m_isFlippedGravity = false;
	bool m_isBlockingSpell = false;
	float m_gravity = 1000.f;
	float m_walkAcceleration = 1500.f;
	float m_configuredGravity = 1000.f;
	float m_dampingGroundPerS = 1.f;
	float m_dampingAirPerS = 0.7f;

	float m_maxVelocityX = 0.f;
	float m_maxVelocityYUp = 0.f;
	float m_maxVelocityYDown = 0.f;

	float m_configuredMaxVelocityX = 0.f;
	float m_configuredMaxVelocityYUp = 0.f;
	float m_configuredMaxVelocityYDown = 0.f;

	// as long as this time is not sf::Time::Zero, the mob will have the fighting animation. 
	sf::Time m_fightAnimationTime = sf::Time::Zero;
	GameObjectState m_fightAnimationState = GameObjectState::Fighting;
};