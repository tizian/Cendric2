#include "Spell.h"
#include "LevelMovableGameObject.h"
#include "LevelMainCharacter.h"

using namespace std;

void Spell::load(const SpellBean& bean, LevelMovableGameObject* mob, sf::Vector2f target, float divergenceAngle)
{
	m_activeCoolDown = bean.duration;
	m_damage = bean.damage;
	m_heal = bean.heal;
	m_damageType = bean.damageType;
	m_reflectCount = bean.reflectCount;
	m_speed = bean.startVelocity;
	m_id = bean.id;
	setBoundingBox(bean.boundingBox);
	setDebugBoundingBox(sf::Color::Red);

	m_level = mob->getLevel();
	m_mob = mob;
	m_ownerType = mob->getConfiguredType();
	m_screen = mob->getScreen();
	m_enemies = m_screen->getObjects(GameObjectType::_Enemy);
	m_mainChar = dynamic_cast<LevelMainCharacter*>(m_screen->getObjects(GameObjectType::_MainCharacter)->at(0));
	
	if (m_mainChar == nullptr)
	{
		g_logger->logError("Spell", "Could not find main character of game screen");
		return;
	}
	
	sf::Vector2f absolutePosition; 
	calculatePositionAccordingToMob(absolutePosition);
	setPosition(absolutePosition);
	
	
	// if the spell is attached to the main char, velocity is ignored 
	if (getConfiguredIsAttachedToMob())
	{
		setVelocity(sf::Vector2f(0, 0));
		return;
	}

 	sf::Vector2f trueDir = target - absolutePosition;
	// normalize dir
	float len = sqrt(trueDir.x * trueDir.x + trueDir.y * trueDir.y);
	trueDir.x = (len == 0) ? 0 : trueDir.x / len;
	trueDir.y = (len == 0) ? 0 : trueDir.y / len;

	sf::Vector2f direction = rotateVector(trueDir, divergenceAngle);

	if (getConfiguredRotateSprite())
	{
		setRotation(atan2(direction.y, direction.x));
	}

	setVelocity(m_speed * direction);
}

bool Spell::getConfiguredIsAttachedToMob() const
{
	return false;
}

void Spell::calculatePositionAccordingToMob(sf::Vector2f& position) const
{
	if (m_mob == nullptr)
	{
		// owner could be dead and looted.
		return;
	}
	sf::Vector2f mainCharPosition(m_mob->getPosition().x + (m_mob->getBoundingBox()->width / 2), m_mob->getPosition().y);
	sf::Vector2f offset;
	if (m_mob->getIsFacingRight())
	{
		offset = sf::Vector2f(getConfiguredPositionOffset() + m_mob->getConfiguredSpellOffset());
	}
	else
	{
		offset = sf::Vector2f(-(getConfiguredPositionOffset().x + m_mob->getConfiguredSpellOffset().x) - getBoundingBox()->width, getConfiguredPositionOffset().y + m_mob->getConfiguredSpellOffset().y);
	}

	position.x = (mainCharPosition + offset).x;
	position.y = (mainCharPosition + offset).y;
}

void Spell::update(const sf::Time& frameTime)
{
	if (getConfiguredIsAttachedToMob())
	{
		calculatePositionAccordingToMob(m_nextPosition);
		setPosition(m_nextPosition);
	}
	else 
	{
		calculateNextPosition(frameTime, m_nextPosition);
	}
	
	checkCollisions(m_nextPosition);
	// check collisions with dynamic tiles
	sf::FloatRect tmp(m_nextPosition, sf::Vector2f(getBoundingBox()->width, getBoundingBox()->height));
	m_level->collideWithDynamicTiles(this, &tmp);
	// check collisions with main char
	if (m_ownerType != GameObjectType::_MainCharacter)
	{
		checkCollisionsWithMainChar(getBoundingBox());
	}
	// check collisions with enemies
	checkCollisionsWithEnemies(getBoundingBox());
	MovableGameObject::update(frameTime);
	
	m_activeCoolDown = m_activeCoolDown - frameTime;

	if (m_activeCoolDown.asMilliseconds() <= 0)
	{
		setDisposed();
	}
}

const sf::Vector2f Spell::getConfiguredPositionOffset() const
{
	return sf::Vector2f(20.f, 0.f);
}

bool Spell::getConfiguredRotateSprite() const
{
	return true;
}

void Spell::checkCollisions(const sf::Vector2f& nextPosition)
{
	sf::FloatRect nextBoundingBoxX(nextPosition.x, getBoundingBox()->top, getBoundingBox()->width, getBoundingBox()->height);
	sf::FloatRect nextBoundingBoxY(getBoundingBox()->left, nextPosition.y, getBoundingBox()->width, getBoundingBox()->height);

	bool isMovingY = nextPosition.y != getBoundingBox()->top;
	bool isMovingX = nextPosition.x != getBoundingBox()->left;
	bool reflected = false;
	// check for collision on x axis
	if (isMovingX && m_level->collidesX(nextBoundingBoxX))
	{
		if (m_reflectCount <= 0)
		{
			setDisposed();
			return;
		}
		else
		{
			reflected = true;
			setAcceleration(sf::Vector2f(0.f, 0.f));
			setVelocityX(-getVelocity().x);
			if (getConfiguredRotateSprite())
			{
				setRotation(atan2(getVelocity().y, getVelocity().x));
			}
		}
	}
	// check for collision on y axis
	if (isMovingY && m_level->collidesY(nextBoundingBoxY))
	{
		if (m_reflectCount <= 0)
		{
			setDisposed();
			return;
		}
		else
		{
			reflected = true;
			setAcceleration(sf::Vector2f(0.f, 0.f));
			setVelocityY(-getVelocity().y);
			if (getConfiguredRotateSprite())
			{
				setRotation(atan2(getVelocity().y, getVelocity().x));
			}
		}
	}
	if (reflected)
	{
		m_reflectCount -= 1;
	}
}

void Spell::checkCollisionsWithMainChar(const sf::FloatRect* boundingBox)
{
	if (m_mainChar->getBoundingBox()->intersects(*boundingBox))
	{
		m_mainChar->onHit(this);
	}
}

void Spell::checkCollisionsWithEnemies(const sf::FloatRect* boundingBox)
{
	for (std::vector<GameObject*>::iterator it = m_enemies->begin(); it != m_enemies->end(); ++it)
	{
		Enemy* enemy = dynamic_cast<Enemy*>((*it));
		if (enemy != nullptr && (enemy->getBoundingBox()->intersects(*boundingBox)))
		{
			enemy->onHit(this);
		}
	}
}

const sf::Time& Spell::getActiveTime() const 
{
	return m_activeCoolDown;
}

GameObjectType Spell::getConfiguredType() const
{
	return GameObjectType::_Spell;
}

DamageType Spell::getDamageType() const
{
	return m_damageType;
}

SpellID Spell::getSpellID() const
{
	return m_id;
}

const MovableGameObject* Spell::getOwner() const
{
	return m_mob;
}

int Spell::getDamage() const
{
	return m_damage;
}

int Spell::getHeal() const
{
	return m_heal;
}

sf::Vector2f Spell::rotateVector(const sf::Vector2f &vec, float angle)
{
	float newX = vec.x * cos(angle) - vec.y * sin(angle);
	float newY = vec.x * sin(angle) + vec.y * cos(angle);
	return sf::Vector2f(newX, newY);
}

