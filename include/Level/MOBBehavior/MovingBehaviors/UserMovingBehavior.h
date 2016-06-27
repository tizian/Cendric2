#pragma once

#include "global.h"
#include "InputController.h"
#include "Level/MOBBehavior/MovingBehavior.h"

// A moving behavior for the level main character
class UserMovingBehavior : public MovingBehavior {
public:
	UserMovingBehavior(LevelMainCharacter* mainChar);
	~UserMovingBehavior() {};

	void update(const sf::Time& frameTime) override;

	void checkCollisions(const sf::Vector2f& nextPosition) override;
	void handleMovementInput() override;
	void handleDefaultAcceleration() override;
	void updateAnimation() override;

private:
	// makes it easier to jump
	static const sf::Time JUMP_GRACE_TIME;
	sf::Time m_jumpGraceTime = sf::Time::Zero;
	// the time the up or down key must be pressed such that the mainchar climbs one step
	static const sf::Time CLIMB_STEP_TIME;
	sf::Time m_climbStepTime = sf::Time::Zero;
	GameObject* m_currentLadder = nullptr;
	bool m_isClimbing = false;
	bool m_isClimbingStep1; // used for the climbing animation
	void handleClimbing(const sf::Time& frameTime);
};