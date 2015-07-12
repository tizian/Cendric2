#include "Enemies/RatEnemy.h"
#include "LevelMainCharacter.h"

RatEnemy::RatEnemy(Level* level, LevelMainCharacter* mainChar) : Enemy(level, mainChar, EnemyID::Rat)
{
	load();
	loadAttributes();
	loadSpells();
	m_jumpHeight = getConfiguredMaxVelocityY() * getConfiguredMaxVelocityY() / (2 * getConfiguredGravityAcceleration());
}

void RatEnemy::loadAttributes()
{
	m_attributes->currentHealthPoints = 40;
	m_attributes->maxHealthPoints = 40;
	m_attributes->physicalMultiplier = 0.9f;
}

void RatEnemy::loadSpells()
{
	SpellBean chopSpell = DEFAULT_CHOP;
	chopSpell.maxActiveTime = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(1000);
	chopSpell.damage = 2;
	chopSpell.boundingBox = sf::FloatRect(0, 0, 30, 30);

	m_spellManager->addSpell(chopSpell);
	m_spellManager->setCurrentSpell(SpellID::Chop);
}

sf::Vector2f RatEnemy::getConfiguredSpellOffset() const
{
	return sf::Vector2f(-10.f, 0.f);
}

void RatEnemy::handleAttackInput()
{
	if (distToMainChar() < 100.f)
	{
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
				m_fightAnimationTime = getConfiguredFightAnimationTime();
			}
		}
	}
}

float RatEnemy::getConfiguredAggroRange() const
{
	return 300.f;
}

float RatEnemy::getConfiguredDistanceToAbyss() const
{
	return 20.f;
}

bool RatEnemy::getConfiguredFleeCondition() const
{
	// what a cowardly rat
	return m_attributes->currentHealthPoints < m_attributes->maxHealthPoints / 3;
}

float RatEnemy::getConfiguredApproachingDistance() const
{
	return 10.f;
}

void RatEnemy::handleMovementInput()
{
	Enemy::handleMovementInput();
}

void RatEnemy::load()
{
	setBoundingBox(sf::FloatRect(0.f, 0.f, 40.f, 30.f));
	setSpriteOffset(sf::Vector2f(-5.f, -20.f));

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

	Animation deadAnimation;
	deadAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_rat));
	deadAnimation.addFrame(sf::IntRect(300, 0, 50, 50));

	addAnimation(GameObjectState::Dead, deadAnimation);

	setFrameTime(sf::seconds(0.08f));

	// initial values
	m_state = GameObjectState::Idle;
	m_isFacingRight = true;
	setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	playCurrentAnimation(true);
}

sf::Time RatEnemy::getConfiguredFightAnimationTime() const
{
	return sf::milliseconds(4 * 80);
}

float RatEnemy::getConfiguredMaxVelocityY() const
{
	return 400.0f;
}

float RatEnemy::getConfiguredMaxVelocityX() const
{
	return 100.0f;
}
