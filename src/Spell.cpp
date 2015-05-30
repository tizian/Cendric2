#include "Spell.h"
#include "LevelMovableGameObject.h"
#include "LevelMainCharacter.h"

using namespace std;

void Spell::loadSpell(Level* level, LevelMovableGameObject* mob, sf::Vector2f target) 
{
	m_level = level;
	m_mob = mob;
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
	
	sf::Vector2f direction = target - absolutePosition;
	// normalize dir
	float len = sqrt(direction.x * direction.x + direction.y * direction.y);
	direction.x = (len == 0) ? 0 : direction.x / len;
	direction.y = (len == 0) ? 0 : direction.y / len;
	
	if (getConfiguredRotateSprite())
	{
		setRotation(atan2(direction.y, direction.x));
	}
	
	// if the spell is attached to the main char, velocity is ignored 
	if (getConfiguredIsAttachedToMob())
	{
		setVelocity(sf::Vector2f(0, 0));
		return;
	}
	
	setVelocity(m_speed * direction);
}

bool Spell::getConfiguredIsAttachedToMob() const
{
	return false;
}

void Spell::calculatePositionAccordingToMob(sf::Vector2f& position) const
{
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

void Spell::init(SpellBean& bean) 
{
	m_activeCoolDown = bean.maxActiveTime;
	m_damage = bean.damage;
	m_reflectCount = bean.reflectCount;
	m_speed = bean.startVelocity;
	setBoundingBox(bean.boundingBox);
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

bool Spell::getConfiguredTriggerFightAnimation() const
{
	return false;
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

	// check for collision on x axis
	if (isMovingX && m_level->collidesX(nextBoundingBoxX))
	{
		setDisposed();
	}
	// check for collision on y axis
	if (isMovingY && m_level->collidesY(nextBoundingBoxY))
	{
		setDisposed();
	}

	// check collisions with dynamic tiles
	m_level->collideWithDynamicTiles(this, nextBoundingBoxX, nextBoundingBoxY);
	// check collisions with main char
	if (!(m_mob->getConfiguredType() == GameObjectType::_MainCharacter))
	{
		checkCollisionsWithMainChar(nextBoundingBoxX, nextBoundingBoxY);
	}
	// check collisions with enemies
	checkCollisionsWithEnemies(nextBoundingBoxX, nextBoundingBoxY);
}

void Spell::checkCollisionsWithMainChar(const sf::FloatRect& nextBoundingBoxX, const sf::FloatRect& nextBoundingBoxY)
{
	if (m_mainChar->getBoundingBox()->intersects(nextBoundingBoxX) || m_mainChar->getBoundingBox()->intersects(nextBoundingBoxY))
	{
		m_mainChar->onHit(this);
	}
}

void Spell::checkCollisionsWithEnemies(const sf::FloatRect& nextBoundingBoxX, const sf::FloatRect& nextBoundingBoxY)
{
	for (std::vector<GameObject*>::iterator it = m_enemies->begin(); it != m_enemies->end(); ++it)
	{
		Enemy* enemy = dynamic_cast<Enemy*>((*it));
		if (enemy != nullptr && (enemy->getBoundingBox()->intersects(nextBoundingBoxX) || enemy->getBoundingBox()->intersects(nextBoundingBoxY)))
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

sf::Color Spell::getConfiguredDebugColor() const
{
	return sf::Color::Red;
}

const MovableGameObject* Spell::getOwner() const
{
	return m_mob;
}

int Spell::getDamage() const
{
	return m_damage;
}

