#include "Enemy.h"
#include "Level.h"
#include "LevelMainCharacter.h"

Enemy::Enemy(Level* level, LevelMainCharacter* mainChar, EnemyID id) : LevelMovableGameObject(level)
{
	m_id = id;
	m_mainChar = mainChar;
	m_immuneEnemies.push_back(id);
	m_attributes = ZERO_ATTRIBUTES;
	m_enemyState = EnemyState::Idle;
	
	// load hp bar
	m_hpBar.setFillColor(sf::Color::Red);
	updateHpBar();
}

bool Enemy::getConfiguredFleeCondition() const
{
	return false;
}

void Enemy::addDamage(int damage)
{
	m_attributes.currentHealthPoints = std::max(0, std::min(m_attributes.maxHealthPoints, m_attributes.currentHealthPoints - damage));
	if (m_attributes.currentHealthPoints == 0)
	{
		m_isDead = true;
	}
}

void Enemy::setDead()
{
	m_attributes.currentHealthPoints = 0;
	m_isDead = true;
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
		// set enemy down.
		setPositionY(m_level->getGround(nextBoundingBoxY));
		m_isGrounded = true;
		if (!m_isDead && m_level->collidesLevelBottom(nextBoundingBoxY))
		{
			// colliding with level bottom is deadly.
			setDead();
		}
	}

	m_jumps = false;
	if (isMovingX && collidesX)
	{
		// would a jump work? if the state is fleeing, the enemy doesn't think about that anymore.
		m_jumps = m_enemyState == EnemyState::Fleeing || !m_level->collidesAfterJump(*getBoundingBox(), m_jumpHeight, m_isFacingRight);
	}

	// checks if the enemy falls would fall deeper than it can jump. fleeing enemies don't consider this and fall anyway.
	if (!(m_enemyState == EnemyState::Fleeing) && isMovingX && m_level->fallsDeep(*getBoundingBox(), m_jumpHeight, m_isFacingRight, getConfiguredDistanceToAbyss()))
	{
		setAccelerationX(0.0f);
		setVelocityX(0.0f);
	}

	if (std::abs(getVelocity().y) > 0.0f)
	{
		m_isGrounded = false;
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
		return;
	}
	addDamage(damage);
	m_recoveringTime = getConfiguredRecoveringTime();
}

void Enemy::render(sf::RenderTarget &renderTarget)
{
	LevelMovableGameObject::render(renderTarget);
	renderTarget.draw(m_hpBar);
	m_animatedSprite.setColor(sf::Color::White);
}

void Enemy::update(const sf::Time& frameTime) 
{
	updateEnemyState(frameTime);
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
	return sqrt(dist.x * dist.x + dist.y * dist.y);
}

sf::Time Enemy::getConfiguredRecoveringTime() const
{
	return sf::milliseconds(200);
}

float Enemy::getConfiguredDistanceToAbyss() const
{
	return 10.f;
}

sf::Time Enemy::getRandomDescisionTime() const
{
	int r = rand() % 1500 + 200;
	return sf::milliseconds(r);
}

GameObjectType Enemy::getConfiguredType() const
{
	return GameObjectType::_Enemy;
}

void Enemy::updateEnemyState(const sf::Time& frameTime)
{
	if (m_isDead) return;

	if (m_recoveringTime > sf::Time::Zero)
	{
		m_enemyState = EnemyState::Recovering;
		m_recoveringTime -= frameTime;
		if (m_recoveringTime < sf::Time::Zero)
		{
			m_recoveringTime = sf::Time::Zero;
			m_enemyState = EnemyState::Idle;
		}
		return;
	}

	if (getConfiguredFleeCondition())
	{
		m_enemyState = EnemyState::Fleeing;
		return;
	}

	if (m_enemyState == EnemyState::Idle)
	{
		m_descisionTime -= frameTime;
		if (m_descisionTime < sf::Time::Zero)
		{
			// decide again
			m_descisionTime = getRandomDescisionTime();
			m_randomDescision = rand() % 3 - 1;
		}
	}

	if (m_enemyState == EnemyState::Idle && distToMainChar() < getConfiguredAggroRange())
	{
		m_enemyState = EnemyState::Chasing;
		return;
	}

	if ((m_enemyState == EnemyState::Chasing || m_enemyState == EnemyState::Fleeing) && distToMainChar() > getConfiguredAggroRange())
	{
		m_enemyState = EnemyState::Idle;
		return;
	}
}

void Enemy::handleMovementInput()
{
	// movement AI
	float newAccelerationX = 0;

	if (m_enemyState == EnemyState::Chasing)
	{
		if (m_mainChar->getCenter().x < getCenter().x && std::abs(m_mainChar->getCenter().x - getCenter().x) > getConfiguredApproachingDistance())
		{
			m_nextIsFacingRight = false;
			newAccelerationX -= getConfiguredWalkAcceleration();
		}
		if (m_mainChar->getCenter().x > getCenter().x && std::abs(m_mainChar->getCenter().x - getCenter().x) > getConfiguredApproachingDistance())
		{
			m_nextIsFacingRight = true;
			newAccelerationX += getConfiguredWalkAcceleration();
		}
		if (m_jumps && m_isGrounded)
		{
			setVelocityY(-getConfiguredMaxVelocityY()); // first jump vel will always be max y vel. 
			m_jumps = false;
		}
	}

	if (m_enemyState == EnemyState::Fleeing)
	{
		if (m_mainChar->getCenter().x < getCenter().x)
		{
			m_nextIsFacingRight = true;
			newAccelerationX += getConfiguredWalkAcceleration();
		}
		if (m_mainChar->getCenter().x > getCenter().x)
		{
			m_nextIsFacingRight = false;
			newAccelerationX -= getConfiguredWalkAcceleration();
		}
		if (m_jumps && m_isGrounded)
		{
			setVelocityY(-getConfiguredMaxVelocityY()); // first jump vel will always be max y vel. 
			m_jumps = false;
		}
	}

	if (m_enemyState == EnemyState::Idle)
	{
		if (m_randomDescision != 0)
		{
			m_nextIsFacingRight = (m_randomDescision == 1);
			newAccelerationX += (m_randomDescision * getConfiguredWalkAcceleration());
		}
	}

	setAcceleration(sf::Vector2f(newAccelerationX, getConfiguredGravityAcceleration()));
}

EnemyID Enemy::getEnemyID() const
{
	return m_id;
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
			m_screen->setTooltipText(g_textProvider->getText("OutOfRange"), sf::Color::Red, true);
		}
	}
}