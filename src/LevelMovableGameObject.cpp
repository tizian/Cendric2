#include "LevelMovableGameObject.h"
#include "SpellManager.h"
#include "Level.h"

LevelMovableGameObject::LevelMovableGameObject(Level* level)
{
	m_level = level;
	m_spellManager = new SpellManager();
	// children should add spells to spell manager in their constructor.
}

LevelMovableGameObject::~LevelMovableGameObject()
{
	delete m_spellManager;
}

void LevelMovableGameObject::update(const sf::Time& frameTime)
{
	if (m_state == GameObjectState::Dead)
	{
		setAcceleration(sf::Vector2f(0, getConfiguredGravityAcceleration()));
	}
	else
	{
		handleInput();
	}
	
	m_spellManager->update(frameTime);
	calculateNextPosition(frameTime, m_nextPosition);
	checkCollisions(m_nextPosition);
	MovableGameObject::update(frameTime);
	m_fightAnimationTime = (m_fightAnimationTime - frameTime) >= sf::Time::Zero ? m_fightAnimationTime - frameTime : sf::Time::Zero;
	updateAnimation();
}

void LevelMovableGameObject::checkCollisions(const sf::Vector2f& nextPosition)
{
	sf::FloatRect nextBoundingBoxX(nextPosition.x, getBoundingBox()->top, getBoundingBox()->width, getBoundingBox()->height);
	sf::FloatRect nextBoundingBoxY(getBoundingBox()->left, nextPosition.y, getBoundingBox()->width, getBoundingBox()->height);

	bool isMovingDown = nextPosition.y > getBoundingBox()->top; // the mob is always moving either up or down, because of gravity. There are very, very rare, nearly impossible cases where they just cancel out.
	bool isMovingX = nextPosition.x != getBoundingBox()->left;

	// check for collision on x axis
	if (isMovingX && m_level->collidesX(nextBoundingBoxX))
	{
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

	if (std::abs(getVelocity().y) > 0.0f)
	{
		m_isGrounded = false;
	}
}

sf::Vector2f LevelMovableGameObject::getConfiguredSpellOffset() const
{
	return sf::Vector2f(0, 0);
}

void LevelMovableGameObject::updateAnimation()
{
	// calculate new game state and set animation.

	GameObjectState newState = GameObjectState::Idle;
	if (m_isDead)
	{
		newState = GameObjectState::Dead;
	}
	else if (m_fightAnimationTime > sf::Time::Zero)
	{
		newState = GameObjectState::Fighting;
	}
	else if (!m_isGrounded)
	{
		newState = GameObjectState::Jumping;
	}
	else if (std::abs(getVelocity().x) > 20.0f)
	{
		newState = GameObjectState::Walking;
	}

	// only update animation if we need to
	if (m_state != newState || m_nextIsFacingRight != m_isFacingRight)
	{
		m_isFacingRight = m_nextIsFacingRight;
		m_state = newState;
		setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	}
}

void LevelMovableGameObject::handleInput()
{
	// nop
}

void LevelMovableGameObject::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const
{
	// distinguish damping in the air and at the ground
	float dampingPerSec = (getVelocity().y == 0.0f) ? getConfiguredDampingGroundPersS() : getConfiguredDampingAirPerS();
	// don't damp when there is active acceleration 
	if (getAcceleration().x != 0.0f) dampingPerSec = 0;
	nextVel.x = (getVelocity().x + getAcceleration().x * frameTime.asSeconds()) * pow(1 - dampingPerSec, frameTime.asSeconds());
	nextVel.y = getVelocity().y + getAcceleration().y * frameTime.asSeconds();
}

Level* LevelMovableGameObject::getLevel() const
{
	return m_level;
}

bool LevelMovableGameObject::getIsFacingRight() const
{
	return m_isFacingRight;
}

GameObjectState LevelMovableGameObject::getState() const
{
	return m_state;
}

float LevelMovableGameObject::getConfiguredWalkAcceleration() const
{
	return 1500.0f;
}

float LevelMovableGameObject::getConfiguredGravityAcceleration() const
{
	return 1000.0f;
}

float LevelMovableGameObject::getConfiguredDampingGroundPersS() const
{
	return 1.f;
}

float LevelMovableGameObject::getConfiguredDampingAirPerS() const
{
	return 0.7f;
}

SpellManager* LevelMovableGameObject::getSpellManager() const
{
	return m_spellManager;
}
