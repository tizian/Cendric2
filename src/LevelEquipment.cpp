#include "LevelEquipment.h"
#include "LevelMainCharacter.h"

LevelEquipment::~LevelEquipment()
{
	g_resourceManager->deleteResource(m_resourceID);
}

void LevelEquipment::calculatePositionAccordingToMainChar(sf::Vector2f& position) const
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

void LevelEquipment::update(const sf::Time& frameTime)
{
	GameObjectState newState = m_mainChar->getState();
	if (newState == GameObjectState::Dead)
	{
		setDisposed();
		return;
	}
	bool newFacingRight = m_mainChar->getIsFacingRight();
	if (m_state != newState || newFacingRight != m_isFacingRight) 
	{
		m_state = newState;
		m_isFacingRight = newFacingRight;
		setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	}
	sf::Vector2f newPosition;
	calculatePositionAccordingToMainChar(newPosition);
	setPosition(newPosition);
	GameObject::update(frameTime);
}

const sf::Vector2f LevelEquipment::getConfiguredPositionOffset() const
{
	return sf::Vector2f(-60.f, -20.f);
}

void LevelEquipment::loadEquipment(LevelMainCharacter* mainChar, LevelEquipmentID id)
{
	m_mainChar = mainChar;
	m_equipmentID = id;

	sf::Vector2f position;
	calculatePositionAccordingToMainChar(position);
	setPosition(position);
}

void LevelEquipment::setTextureID(ResourceID id)
{
	m_resourceID = id;
}

GameObjectType LevelEquipment::getConfiguredType() const
{
	return GameObjectType::_LevelEquipment;
}
