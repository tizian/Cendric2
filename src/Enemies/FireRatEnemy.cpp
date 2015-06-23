#include "Enemies/FireRatEnemy.h"
#include "LevelMainCharacter.h"

FireRatEnemy::FireRatEnemy(Level* level, LevelMainCharacter* mainChar) : Enemy(level, mainChar, EnemyID::FireRat)
{
	load();
	loadAttributes();
	loadSpells();
}

void FireRatEnemy::loadAttributes()
{
	m_attributes.currentHealthPoints = 70;
	m_attributes.maxHealthPoints = 70;
	m_attributes.resistanceIce = -10;

	m_immuneSpells.push_back(SpellID::Fire);
}

void FireRatEnemy::loadSpells()
{
	SpellBean chopSpell = DEFAULT_CHOP;
	chopSpell.damage = 15;
	chopSpell.maxActiveTime = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(1000);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 30, 30);

	SpellBean fireSpell = DEFAULT_FIRE;
	fireSpell.damage = 10;
	fireSpell.cooldown = sf::milliseconds(3000);
	fireSpell.startVelocity = 200.f;

	m_spellManager->addSpell(chopSpell);
	m_spellManager->addSpell(fireSpell);
	m_spellManager->setCurrentSpell(SpellID::Chop);
}

void FireRatEnemy::checkCollisions(const sf::Vector2f& nextPosition)
{
	Enemy::checkCollisions(nextPosition);
	// TODO check positions with spells
}

sf::Vector2f FireRatEnemy::getConfiguredSpellOffset() const
{
	return sf::Vector2f(-10.f, 0.f);
}

void FireRatEnemy::handleInput()
{
	// movement AI
	float newAccelerationX = 0;

	if (distToMainChar() < 600.f)
	{
        if (m_mainChar->getCenter().x < getCenter().x && std::abs(m_mainChar->getCenter().x - getCenter().x) > 5)
		{
			m_nextIsFacingRight = false;
			newAccelerationX -= getConfiguredWalkAcceleration();
		}
        if (m_mainChar->getCenter().x > getCenter().x && std::abs(m_mainChar->getCenter().x - getCenter().x) > 5)
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
	setAcceleration(sf::Vector2f(newAccelerationX, getConfiguredGravityAcceleration()));

	// handle attack input
	if (distToMainChar() < 600.f)
	{	
		m_spellManager->setCurrentSpell(SpellID::Fire);
		if (distToMainChar() < 100.f) 
		{
			m_spellManager->setCurrentSpell(SpellID::Chop);
		}
		
		std::vector<Spell*> holder = m_spellManager->getSpells();

		if (!holder.empty())
		{
			int div = 0;
			int sign = 1;
			for (auto& it : holder)
			{
				it->load(getLevel(), this, m_mainChar->getCenter(), div * sign);
				m_screen->addObject(GameObjectType::_Spell, it);
				sign = -sign;
				if (sign == -1)
				{
					div += 1;
				}
			}
			if (holder.at(0)->getConfiguredTriggerFightAnimation()) {
				m_fightAnimationTime = sf::milliseconds(4 * 80); // duration of fight animation
			}
		}
	}
}

void FireRatEnemy::load()
{
	setBoundingBox(sf::FloatRect(0.f, 0.f, 40.f, 30.f));
	setSpriteOffset(sf::Vector2f(-5.f, -20.f));

	Animation walkingAnimation;
	walkingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_firerat));
	walkingAnimation.addFrame(sf::IntRect(0, 0, 50, 50));
	walkingAnimation.addFrame(sf::IntRect(50, 0, 50, 50));
	walkingAnimation.addFrame(sf::IntRect(100, 0, 50, 50));
	walkingAnimation.addFrame(sf::IntRect(50, 0, 50, 50));

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_firerat));
	idleAnimation.addFrame(sf::IntRect(50, 0, 50, 50));
	idleAnimation.addFrame(sf::IntRect(300, 0, 50, 50));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation jumpingAnimation;
	jumpingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_firerat));
	jumpingAnimation.addFrame(sf::IntRect(150, 0, 50, 50));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation fightingAnimation;
	fightingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_firerat));
	fightingAnimation.addFrame(sf::IntRect(200, 0, 50, 50));
	fightingAnimation.addFrame(sf::IntRect(250, 0, 50, 50));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation deadAnimation;
	deadAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_firerat));
	deadAnimation.addFrame(sf::IntRect(350, 0, 50, 50));

	addAnimation(GameObjectState::Dead, deadAnimation);

	setFrameTime(sf::seconds(0.08f));

	// initial values
	m_state = GameObjectState::Idle;
	m_isFacingRight = true;
	setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	playCurrentAnimation(true);
}

float FireRatEnemy::getConfiguredMaxVelocityY() const
{
	return 400.0f;
}

float FireRatEnemy::getConfiguredMaxVelocityX() const
{
	return 50.0f;
}
