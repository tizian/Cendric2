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
	virtual void updateAnimation(const sf::Time& frameTime) = 0;

	virtual void checkCollisions(const sf::Vector2f& nextPosition) = 0;
	
	// the mob ignores collidable dynamic tiles in its collision logic but still collides with strictly dynamic tiles
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const;

	virtual void handleMovementInput() = 0;
	virtual void handleDefaultAcceleration();

	// gravity flip (used for anti gravity spell)
	virtual void flipGravity();
	// change gravity (used for leap of faith spell)
	void setGravityScale(float scale);
	// change x speed (used for ghost spell)
	void setMaxXVelocityScale(float scale);
	// the mob ignores collidable dynamic tiles in its collision logic but still collides with strictly dynamic tiles
	virtual void setIgnoreDynamicTiles(bool value);
	// recovers from all fighting animations/fighting times/casting times and is ready again (cooldowns don't recover and climbing properties don't, too)
	void setReady();
	// set enabled/disabled
	virtual void setEnabled(bool enabled);
	// set collisions enabled/disabled
	void setCollisionsEnabled(bool enabled);

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
	void executeFightAnimation(const sf::Time& animationTime, GameObjectState animation, bool isBlocking);
	// returns the time this animation takes
	sf::Time executeDefaultFightAnimation(bool isBlocking, int times = 1);
	void setDefaultFightAnimation(const sf::Time& animationTime, GameObjectState animation);
	virtual void setFacingRight(bool value);
	void setCollisionTiltSuppressed();

	float getGravity() const;
	bool isGrounded() const;
	bool isFacingRight() const;
	virtual bool isUpsideDown() const;
	virtual bool isReady() const;
	bool isIgnoreDynamicTiles() const;
	
protected:
	// helper method used for collision checking
	void checkXYDirection(const sf::Vector2f& nextPosition, bool& collidesX, bool& collidesY);
	// if the position delta is too high (> tileSize), the mob is killed and no further collisions are checked.
	void checkForCollisionTilt(const sf::Vector2f& oldPosition, float oldVelocity);
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
	bool m_isEnabled = true;
	bool m_isCollisionEnabled = true;
	bool m_isCollidingX = false;
	bool m_isCollidingY = false;
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

	sf::Time m_defaultFightAnimationTime = sf::Time::Zero;
	GameObjectState m_defaultFightAnimationState = GameObjectState::Fighting;
};