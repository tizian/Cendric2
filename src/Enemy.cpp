#include "Enemy.h"
#include "Level.h"
#include "LevelMainCharacter.h"

Enemy::Enemy(Level* level, LevelMainCharacter* mainChar) : LevelMovableGameObject(level)
{
	m_mainChar = mainChar;
}

Enemy::~Enemy()
{
	delete m_spellManager;
}

void Enemy::checkCollisions(const sf::Vector2f& nextPosition)
{
	sf::FloatRect nextBoundingBoxX(nextPosition.x, getBoundingBox()->top, getBoundingBox()->width, getBoundingBox()->height);
	sf::FloatRect nextBoundingBoxY(getBoundingBox()->left, nextPosition.y, getBoundingBox()->width, getBoundingBox()->height);

	bool isMovingDown = nextPosition.y > getBoundingBox()->top; // the mob is always moving either up or down, because of gravity. There are very, very rare, nearly impossible cases where they just cancel out.
	bool isMovingX = nextPosition.x != getBoundingBox()->left;

	// check for collision on x axis
	bool collidesX = false;
	if (isMovingX && m_level->collidesX(nextBoundingBoxX))
	{
		collidesX = true;
		setAccelerationX(0.0f);
		setVelocityX(0.0f);
	}
	// check for collision on y axis
	bool collidesY = m_level->collidesY(nextBoundingBoxY);
	if (!isMovingDown && collidesY)
	{
		setAccelerationY(0.0);
		setVelocityY(0.0f);
	}
	else if (isMovingDown && collidesY)
	{
		setAccelerationY(0.0f);
		setVelocityY(0.0f);
		// set cendric down.
		setPositionY(m_level->getGround(nextBoundingBoxY));
		m_isGrounded = true;
	}

	if (abs(getVelocity().y) > 0.0f)
	{
		m_isGrounded = false;
	}

	m_jumps = false;
	if (isMovingX && collidesX)
	{
		m_jumps = true;
	}
	// TODO check positions with spells referencing not this object.
}

float Enemy::distToMainChar() const
{
	sf::Vector2f dist = m_mainChar->getCenter() - getCenter();
	return (sqrt(dist.x * dist.x + dist.y * dist.y));
}

GameObjectType Enemy::getConfiguredType() const
{
	return GameObjectType::_Enemy;
}

sf::Color Enemy::getConfiguredDebugColor() const
{
	return sf::Color::Magenta;
}
