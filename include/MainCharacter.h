#pragma once

#include "global.h"
#include "MovableGameObject.h"
#include "Level.h"
#include "InputController.h"
#include "SpellManager.h"
#include "Screen.h"
#include "IceStaff.h"

// Cendric in a level
class MainCharacter : public MovableGameObject
{
public:
	MainCharacter(Level* level);
	~MainCharacter();

	void load() override;
	void update(sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) const override;
	void checkCollisions(sf::Vector2f nextPosition) override;
	void calculateUnboundedVelocity(sf::Time& frameTime, sf::Vector2f& nextVel) override;

	const float getConfiguredMaxVelocityY() override;
	const float getConfiguredMaxVelocityX() override;
	GameObjectType getConfiguredType() const override;

	Level* getLevel();
	Spell* getFiredSpell();
	bool getIsFacingRight();
	GameObjectState getState();

private:
	const float WALK_ACCELERATION = 1500.0f;
	const float GRAVITY_ACCELERATION = 1000.0f;
	// choose a value between 0.9 for really slow halting and 1.0f for aprupt halting.
	const float DAMPING_GROUND_PER_S = 0.999f;
	const float DAMPING_AIR_PER_S = 0.7f;

	// handle input and calculate the next position
	void handleInput();
	// update animation based on the current velocity + grounded
	void updateAnimation();
	bool m_isFacingRight;
	bool m_nextIsFacingRight;
	bool m_isGrounded;
	Level* m_level;
	Spell* m_firedSpell;
	Weapon* m_weapon;

	SpellManager* m_spellManager;
	// as long as this time is not null, cendric will have the fighting animation. 
	sf::Time m_fightAnimationTime;
};