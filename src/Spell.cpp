#include "Spell.h"
#include "MainCharacter.h"

void Spell::loadSpell(Level* level, MainCharacter* mainChar)
{
	m_level = level;
	m_mainChar = mainChar;
	
	sf::Vector2f absolutePosition; 
	calculatePositionAccordingToMainChar(absolutePosition);
	setPosition(absolutePosition);

	// if the spell is attached to the main char, velocity is ignored
	if (getConfiguredIsAttachedToMainChar())
	{
		setVelocity(sf::Vector2f(0, 0));
		return;
	}
	sf::Vector2f direction = g_inputController->getMousePosition() - absolutePosition;
	// normalize dir
	float len = sqrt(direction.x * direction.x + direction.y * direction.y);
	direction.x = (len == 0) ? 0 : direction.x / len;
	direction.y = (len == 0) ? 0 : direction.y / len;
	
	setVelocity(m_speed * direction);
}

bool Spell::getConfiguredIsAttachedToMainChar()
{
	return false;
}

void Spell::calculatePositionAccordingToMainChar(sf::Vector2f& position)
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

void Spell::init(SpellBean& bean) 
{
	m_isDisposed = false;
	m_activeCoolDown = bean.maxActiveTime;
	m_damage = bean.damage;
	m_reflectCount = bean.reflectCount;
	m_speed = bean.startVelocity;
	load();
}

void Spell::update(sf::Time& frameTime)
{
	if (getConfiguredIsAttachedToMainChar())
	{
		calculatePositionAccordingToMainChar(m_nextPosition);
		setPosition(m_nextPosition);
	}
	else 
	{
		calculateNextPosition(frameTime, m_nextPosition);
	}

	MovableGameObject::update(frameTime);
	checkCollisions(m_nextPosition);
	m_activeCoolDown = m_activeCoolDown - frameTime;

	if (m_activeCoolDown.asMilliseconds() <= 0)
	{
		setDisposed();
	}
}

sf::Vector2f Spell::getConfiguredPositionOffset()
{
	return sf::Vector2f(20.f, 0.f);
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

bool Spell::isDisposed()
{
	return m_isDisposed;
}

void Spell::setDisposed()
{
	m_isDisposed = true;
}

