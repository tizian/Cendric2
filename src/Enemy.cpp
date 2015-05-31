#include "Enemy.h"
#include "Level.h"
#include "LevelMainCharacter.h"

Enemy::Enemy(Level* level, LevelMainCharacter* mainChar, EnemyID id) : LevelMovableGameObject(level)
{
	m_id = id;
	m_mainChar = mainChar;
	m_immuneEnemies.push_back(id);
	m_attributes = ZERO_ATTRIBUTES;
	
	// load hp bar
	m_hpBar.setFillColor(sf::Color::Red);
	updateHpBar();
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
}

void Enemy::onHit(Spell* spell)
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
	// check for immune spells, if yes, the spell will disappear, absorbed by the immuneness of this enemy
	if (std::find(m_immuneSpells.begin(), m_immuneSpells.end(), spell->getConfiguredSpellID()) != m_immuneSpells.end())
	{
		spell->setDisposed();
		return;
	}
	int damage = 0;
	switch (spell->getConfiguredSpellID())
	{
	case SpellID::Ice:
		damage = spell->getDamage() - m_attributes.resistanceIce;
		spell->setDisposed();
		break;
	case SpellID::Fire:
		damage = spell->getDamage() - m_attributes.resistanceFire;
		spell->setDisposed();
		break;
	case SpellID::Chop:
		damage = spell->getDamage() - m_attributes.resistancePhysical;
		spell->setDisposed();
		break;
	default:
		break;
	}
	m_attributes.currentHealthPoints = m_attributes.currentHealthPoints - std::max(damage, 0);
	if (m_attributes.currentHealthPoints < 0)
	{
		m_attributes.currentHealthPoints = 0;
		m_isDead = true;
	}
}

void Enemy::render(sf::RenderTarget &renderTarget)
{
	LevelMovableGameObject::render(renderTarget);
	renderTarget.draw(m_hpBar);
	m_animatedSprite.setColor(sf::Color::White);
}

void Enemy::update(const sf::Time& frameTime) 
{
	LevelMovableGameObject::update(frameTime);
	updateHpBar();
}

void Enemy::updateHpBar() 
{
	m_hpBar.setPosition(getBoundingBox()->left, getBoundingBox()->top - getConfiguredDistanceToHPBar());
	m_hpBar.setSize(sf::Vector2f(getBoundingBox()->width * (static_cast<float>(m_attributes.currentHealthPoints) / m_attributes.maxHealthPoints), HP_BAR_HEIGHT));
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

EnemyID Enemy::getEnemyID() const
{
	return m_id;
}

sf::Color Enemy::getConfiguredDebugColor() const
{
	return sf::Color::Magenta;
}

float Enemy::getConfiguredDistanceToHPBar() const
{
	return 20.f;
}

void Enemy::setLoot(const std::map<ItemID, int>& items, int gold)
{
	m_lootableItems = items;
	m_lootableGold = gold;
}

void Enemy::onMouseOver()
{
	if (m_state == GameObjectState::Dead)
	{
		m_animatedSprite.setColor(sf::Color::Red);
	}
}

void Enemy::onRightClick()
{
	if (m_state == GameObjectState::Dead)
	{
		// check if the enemy body is in range
		sf::Vector2f dist = m_mainChar->getCenter() - getCenter();
		if (sqrt(dist.x * dist.x + dist.y * dist.y) <= PICKUP_RANGE)
		{
			// loot, create the correct items + gold in the players inventory.
			m_mainChar->lootItems(m_lootableItems);
			m_mainChar->addGold(m_lootableGold);
			setDisposed();
		}
		else
		{
			m_screen->setTooltipText(g_textProvider->getText(Texts::Warn_itemTooFarAway), sf::Vector2f(10.f, 10.f), sf::Color::Red, true);
		}
	}
}