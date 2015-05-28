#include "Enemies/RatEnemy.h"
#include "LevelMainCharacter.h"

RatEnemy::RatEnemy(Level* level, LevelMainCharacter* mainChar) : Enemy(level, mainChar, EnemyID::Rat)
{
	load();
	loadAttributes();
	loadSpells();
}

void RatEnemy::loadAttributes()
{
	m_attributes.currentHealthPoints = 40;
	m_attributes.maxHealthPoints = 40;
	m_attributes.resistancePhysical = 10;
}

void RatEnemy::loadSpells()
{
	SpellBean chopSpell;
	chopSpell.id = SpellID::Chop;
	chopSpell.maxActiveTime = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(1000);
	chopSpell.damage = 2;
	chopSpell.reflectCount = 0;
	chopSpell.startVelocity = 0.f;
	chopSpell.boundingBox = sf::FloatRect(0, 0, 30, 30);

	m_spellManager->addSpell(chopSpell);
	m_spellManager->setCurrentSpell(SpellID::Chop);
}

void RatEnemy::checkCollisions(const sf::Vector2f& nextPosition)
{
	Enemy::checkCollisions(nextPosition);
	// TODO check positions with spells
}

sf::Vector2f RatEnemy::getConfiguredSpellOffset() const
{
	return sf::Vector2f(-10.f, 0.f);
}

void RatEnemy::handleInput()
{
	// movement AI
	float newAccelerationX = 0;

	if (distToMainChar() < 500.f)
	{
		if (m_mainChar->getCenter().x < getCenter().x && abs(m_mainChar->getCenter().x - getCenter().x) > 5)
		{
			m_nextIsFacingRight = false;
			newAccelerationX -= getConfiguredWalkAcceleration();
		}
		if (m_mainChar->getCenter().x > getCenter().x && abs(m_mainChar->getCenter().x - getCenter().x) > 5)
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
	if (distToMainChar() < 100.f)
	{
		Spell* spell = m_spellManager->getSpell();
		if (spell != nullptr)
		{
			spell->loadSpell(getLevel(), this, m_mainChar->getCenter());
			if (spell->getConfiguredTriggerFightAnimation()) {
				m_fightAnimationTime = sf::milliseconds(4 * 80); // duration of fight animation
			}
			m_screen->addObject(GameObjectType::_Spell, spell);
		}
	}
}

void RatEnemy::load()
{
	setBoundingBox(sf::FloatRect(0.f, 0.f, 50.f, 30.f));
	setSpriteOffset(sf::Vector2f(0.f, -20.f));

	Animation walkingAnimation;
	walkingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_rat));
	walkingAnimation.addFrame(sf::IntRect(0, 0, 50, 50));
	walkingAnimation.addFrame(sf::IntRect(50, 0, 50, 50));
	walkingAnimation.addFrame(sf::IntRect(100, 0, 50, 50));
	walkingAnimation.addFrame(sf::IntRect(50, 0, 50, 50));

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_rat));
	idleAnimation.addFrame(sf::IntRect(50, 0, 50, 50));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation jumpingAnimation;
	jumpingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_rat));
	jumpingAnimation.addFrame(sf::IntRect(150, 0, 50, 50));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation fightingAnimation;
	fightingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_rat));
	fightingAnimation.addFrame(sf::IntRect(200, 0, 50, 50));
	fightingAnimation.addFrame(sf::IntRect(250, 0, 50, 50));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	setFrameTime(sf::seconds(0.08f));

	// initial values
	m_state = GameObjectState::Idle;
	m_isFacingRight = true;
	setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	playCurrentAnimation(true);
}

float RatEnemy::getConfiguredMaxVelocityY() const
{
	return 400.0f;
}

float RatEnemy::getConfiguredMaxVelocityX() const
{
	return 100.0f;
}
