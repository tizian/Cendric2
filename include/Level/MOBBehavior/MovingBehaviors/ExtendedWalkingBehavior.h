#pragma once

#include "global.h"
#include "Level/MOBBehavior/MovingBehaviors/WalkingBehavior.h"

// A special moving behavior for walking enemies.
// with jumping_start and fighting_start game object states.
class ExtendedWalkingBehavior : public WalkingBehavior {
public:
	ExtendedWalkingBehavior(Enemy* enemy);
	~ExtendedWalkingBehavior() {};

	void update(const sf::Time& frameTime) override;
	void updateAnimation() override;

	void setJumpStartTime(const sf::Time& time);
	void setFightStartTime(const sf::Time& time);

private:
	sf::Time m_jumpStartTime = sf::Time::Zero;
	sf::Time m_configuredJumpStartTime = sf::Time::Zero;
	sf::Time m_fightStartTime = sf::Time::Zero;
	sf::Time m_configuredFightStartTime = sf::Time::Zero;
};