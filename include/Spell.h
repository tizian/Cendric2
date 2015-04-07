#pragma once

#include "global.h"
#include "MovableGameObject.h"
#include "InputController.h"
#include "MainCharacter.h"

// A spell cendric can cast
class Spell : public MovableGameObject
{
public:

	// the direction should be a normalized vector.
	virtual void load(MainCharacter* mainChar, sf::Vector2f direction);
	void update(sf::Time& frameTime) override;
	void checkCollisions(sf::Vector2f nextPosition) override;
	
	// TODO: adds a modifier to the spell
	//void addModifier(Modifier& modifier);

	// the offset of the spells start position, as seen from the upper left corner of cendrics bounding box.
	virtual sf::Vector2f getConfiguredPositionOffset() = 0;
	
	// these values are implemented by the subclasses. 
	virtual int getConfiguredDamage() = 0;
	virtual sf::Vector2f getConfiguredSpeed() = 0;
	virtual sf::Time getConfiguredCoolDown() = 0;
	virtual int getConfiguredReflectCount() = 0;

	// returns whether the spell object should be deleted
	void isDisposed();

private:
	sf::Vector2f m_nextPosition;
	MainCharacter* m_mainChar;
	Level* m_level;
	bool m_isDisposed;
};