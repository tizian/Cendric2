#include "LevelMovableGameObject.h"
#include "SpellManager.h"
#include "Level.h"

LevelMovableGameObject::LevelMovableGameObject(Level* level) : MovableGameObject()
{
	m_level = level;
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
		handleMovementInput();
		handleAttackInput();
	}
	
	m_spellManager->update(frameTime);
	calculateNextPosition(frameTime, m_nextPosition);
	checkCollisions(m_nextPosition);
	m_level->collideWithDynamicTiles(this, getBoundingBox());
	MovableGameObject::update(frameTime);
	// update time
	m_fightAnimationTime = (m_fightAnimationTime - frameTime) >= sf::Time::Zero ? m_fightAnimationTime - frameTime : sf::Time::Zero;
	if (m_coloredTime != sf::Time::Zero)
	{
		m_coloredTime -= frameTime;
		if (m_coloredTime <= sf::Time::Zero)
		{
			m_animatedSprite.setColor(sf::Color::White);
			m_coloredTime = sf::Time::Zero;
		}
	}
	updateAnimation();
	if (!m_isDead)
	{
		updateRegeneration(frameTime);
	}
}

void LevelMovableGameObject::updateRegeneration(const sf::Time& frameTime)
{
	m_timeSinceRegeneration += frameTime;
	if (m_timeSinceRegeneration >= sf::seconds(1))
	{
		m_timeSinceRegeneration -= sf::seconds(1);
		m_attributes->currentHealthPoints += m_attributes->healthRegenerationPerS;
		if (m_attributes->currentHealthPoints > m_attributes->maxHealthPoints)
		{
			m_attributes->currentHealthPoints = m_attributes->maxHealthPoints;
		}
	}
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
		// set mob down.
		setPositionY(m_level->getGround(nextBoundingBoxY));
		m_isGrounded = true;
		if (!m_isDead && m_level->collidesLevelBottom(nextBoundingBoxY))
		{
			// colliding with level bottom is deadly.
			setDead();
		}
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

void LevelMovableGameObject::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const
{
	// distinguish damping in the air and at the ground
	float dampingPerSec = (getVelocity().y == 0.0f) ? getConfiguredDampingGroundPersS() : getConfiguredDampingAirPerS();
	// don't damp when there is active acceleration 
	if (getAcceleration().x != 0.0f) dampingPerSec = 0;
	nextVel.x = (getVelocity().x + getAcceleration().x * frameTime.asSeconds()) * pow(1 - dampingPerSec, frameTime.asSeconds());
	nextVel.y = getVelocity().y + getAcceleration().y * frameTime.asSeconds();
}

void LevelMovableGameObject::addDamage(int damage)
{
	if (m_state == GameObjectState::Dead) return;
	m_attributes->currentHealthPoints = std::max(0, std::min(m_attributes->maxHealthPoints, m_attributes->currentHealthPoints - damage));
	if (m_attributes->currentHealthPoints == 0)
	{
		m_isDead = true;
	}
	setSpriteColor(sf::Color::Red, sf::milliseconds(100));
}

void LevelMovableGameObject::onHit(Spell* spell)
{
	if (m_state == GameObjectState::Dead)
	{
		return;
	}
	// check for owner
	if (spell->getOwner() == this)
	{
		return;
	}
	int damage = 0;
	switch (spell->getDamageType())
	{
	case DamageType::Physical:
		damage = static_cast<int>(spell->getDamage() * m_attributes->physicalMultiplier);
		spell->setDisposed();
		break;
	case DamageType::Ice:
		damage = static_cast<int>(spell->getDamage() * m_attributes->iceMultiplier);
		spell->setDisposed();
		break;
	case DamageType::Fire:
		damage = static_cast<int>(spell->getDamage() * m_attributes->fireMultiplier);
		spell->setDisposed();
		break;
	case DamageType::Shadow:
		damage = static_cast<int>(spell->getDamage() * m_attributes->shadowMultiplier);
		spell->setDisposed();
		break;
	case DamageType::Light:
		damage = static_cast<int>(spell->getDamage() * m_attributes->lightMultiplier);
		spell->setDisposed();
		break;
	default:
		return;
	}
	addDamage(damage);
}

void LevelMovableGameObject::setDead()
{
	m_attributes->currentHealthPoints = 0;
	m_isDead = true;
}

void LevelMovableGameObject::setFightAnimationTime()
{
	m_fightAnimationTime = getConfiguredFightAnimationTime();
}

Level* LevelMovableGameObject::getLevel() const
{
	return m_level;
}

bool LevelMovableGameObject::getIsFacingRight() const
{
	return m_isFacingRight;
}

bool LevelMovableGameObject::isDead() const
{
	return m_isDead;
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

const AttributeBean* LevelMovableGameObject::getAttributes() const
{
	return m_attributes;
}

void LevelMovableGameObject::setSpriteColor(const sf::Color& color, const sf::Time& time)
{
	if (time <= sf::Time::Zero) return;
	m_animatedSprite.setColor(color);
	m_coloredTime = time;
}
