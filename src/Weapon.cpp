#include "Weapon.h"
#include "MainCharacter.h"

void Weapon::calculatePositionAccordingToMainChar(sf::Vector2f& position)
{
	sf::Vector2f mainCharPosition(m_mainChar->getPosition().x + (m_mainChar->getBoundingBox()->width / 2), m_mainChar->getPosition().y);
	sf::Vector2f offset;
	if (m_mainChar->getIsFacingRight())
	{
		offset = sf::Vector2f(getConfiguredPositionOffset());
	}
	else
	{
		offset = sf::Vector2f(-getConfiguredPositionOffset().x - getBoundingBox()->width, getConfiguredPositionOffset().y);
	}

	position.x = (mainCharPosition + offset).x;
	position.y = (mainCharPosition + offset).y;
}

void Weapon::update(sf::Time& frameTime)
{
	GameObjectState newState = m_mainChar->getState();
	bool newFacingRight = m_mainChar->getIsFacingRight();
	if (m_state != newState || newFacingRight != m_isFacingRight) {
		m_state = newState;
		m_isFacingRight = newFacingRight;
		setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	}
	sf::Vector2f newPosition;
	calculatePositionAccordingToMainChar(newPosition);
	setPosition(newPosition);
	GameObject::update(frameTime);
}

sf::Vector2f Weapon::getConfiguredPositionOffset()
{
	return sf::Vector2f(-60.f, -20.f);
}

void Weapon::loadWeapon(MainCharacter* mainChar)
{
	m_mainChar = mainChar;

	sf::Vector2f position;
	calculatePositionAccordingToMainChar(position);
	setPosition(position);
}

GameObjectType Weapon::getConfiguredType() const
{
	return GameObjectType::_Weapon;
}
