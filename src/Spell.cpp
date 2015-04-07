#include "Spell.h"

Spell::Spell(MainCharacter* mainChar, sf::Vector2f startAcceleration)
{
	MovableGameObject::MovableGameObject();
	m_isDisposed = false;
	m_mainChar = mainChar;
	m_level = mainChar->getLevel();
	load();
	setPosition(mainChar->getPosition() + getConfiguredPositionOffset());
	setAcceleration(startAcceleration);
}

Spell::~Spell()
{
	// spell texture is deleted when a level is.
}

void Spell::update(sf::Time& frameTime)
{
	calculateNextPosition(frameTime, m_nextPosition);
	checkCollisions(m_nextPosition);
	MovableGameObject::update(frameTime);
}

void Spell::checkCollisions(sf::Vector2f nextPosition)
{
	sf::FloatRect nextBoundingBoxX(nextPosition.x, getBoundingBox()->top, getBoundingBox()->width, getBoundingBox()->height);
	sf::FloatRect nextBoundingBoxY(getBoundingBox()->left, nextPosition.y, getBoundingBox()->width, getBoundingBox()->height);

	bool isMovingY = nextPosition.y != getBoundingBox()->top;
	bool isMovingX = nextPosition.x != getBoundingBox()->left;

	// check for collision on x axis
	if (isMovingX && m_level->collidesX(nextBoundingBoxX))
	{
		m_isDisposed = true;
	}
	// check for collision on y axis
	if (isMovingY && m_level->collidesY(nextBoundingBoxY))
	{
		m_isDisposed = true;
	}
}

void Spell::load()
{
	// nop
}

