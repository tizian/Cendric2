#pragma once

#include "global.h"
#include "MovableGameObject.h"
#include "InputController.h"
#include "MainCharacter.h"

// A spell cendric can cast
class Spell : public MovableGameObject
{
public:
	Spell(MainCharacter* mainChar, sf::Vector2f startAcceleration);
	~Spell();

	void load() override;
	void update(sf::Time& frameTime) override;
	void checkCollisions(sf::Vector2f nextPosition) override;
	// the offset of the spells start position, as seen from the upper left corner of cendrics bounding box.
	virtual sf::Vector2f getConfiguredPositionOffset();
	// returns whether the spell object should be deleted
	void isDisposed();

private:
	sf::Vector2f m_nextPosition;
	MainCharacter* m_mainChar;
	Level* m_level;
	bool m_isDisposed;
};