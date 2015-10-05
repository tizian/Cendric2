#include "Enemies/NekomataEnemy.h"
#include "LevelMainCharacter.h"

NekomataEnemy::NekomataEnemy(Level* level, LevelMainCharacter* mainChar) : Enemy(level, mainChar, EnemyID::FireRat)
{
	load();
	loadAttributes();
	loadSpells();
	m_jumpHeight = getConfiguredMaxVelocityY() * getConfiguredMaxVelocityY() / (2 * getConfiguredGravityAcceleration());
}

void NekomataEnemy::loadAttributes()
{
	m_attributes.setHealth(300);
	m_attributes.resistanceLight = -30;
	m_attributes.calculateAttributes();

	m_immuneDamageTypes.push_back(DamageType::Shadow);
}

void NekomataEnemy::loadSpells()
{
	SpellBean chopSpell = SpellBean::getSpellBean(SpellID::Chop);
	chopSpell.damage = 50;
	chopSpell.duration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(1000);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 100, 50);

	SpellBean fireBallSpell = SpellBean::getSpellBean(SpellID::FireBall);
	fireBallSpell.damage = 100;
	fireBallSpell.cooldown = sf::milliseconds(3000);
	fireBallSpell.startVelocity = 200.f;

	m_spellManager->addSpell(chopSpell);
	m_spellManager->addSpell(fireBallSpell);
	m_spellManager->setCurrentSpell(0); // chop
}

sf::Vector2f NekomataEnemy::getConfiguredSpellOffset() const
{
	return sf::Vector2f(-10.f, 0.f);
}

void NekomataEnemy::handleAttackInput()
{
	if (m_enemyState != EnemyState::Chasing) return;
	if (distToMainChar() < getAggroRange())
	{
		m_spellManager->setCurrentSpell(1); // fire ball
		if (distToMainChar() < 150.f)
		{
			m_spellManager->setCurrentSpell(0); // chop
		}

		m_spellManager->executeCurrentSpell(m_mainChar->getCenter());
	}
}

void NekomataEnemy::handleMovementInput()
{
	Enemy::handleMovementInput();
	// handle attack input

}

void NekomataEnemy::load()
{
	setBoundingBox(sf::FloatRect(0.f, 0.f, 130.f, 70.f));
	setSpriteOffset(sf::Vector2f(-10.f, -11.f));

	Animation walkingAnimation;
	walkingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));

	for (int i = 0; i < 12; i++) 
	{
		walkingAnimation.addFrame(sf::IntRect(0, i * 81, 150, 81));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation jumpingAnimation;
	jumpingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));
	for (int i = 0; i < 1; i++)
	{
		jumpingAnimation.addFrame(sf::IntRect(150, i * 81, 150, 81));
	}

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation jumpingStartAnimation;
	jumpingStartAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));
	for (int i = 1; i < 4; i++)
	{
		jumpingStartAnimation.addFrame(sf::IntRect(150, i * 81, 150, 81));
	}

	addAnimation(GameObjectState::Jumping_start, jumpingStartAnimation);

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));
	for (int i = 0; i < 1; i++) 
	{
		idleAnimation.addFrame(sf::IntRect(300, i * 81, 150, 81));
	}

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation fightingStartAnimation;
	fightingStartAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));
	for (int i = 0; i < 1; i++)
	{
		fightingStartAnimation.addFrame(sf::IntRect(450, i * 81, 150, 81));
	}

	addAnimation(GameObjectState::Fighting_start, fightingStartAnimation);

	Animation fightingAnimation;
	fightingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));
	fightingAnimation.addFrame(sf::IntRect(450, 1 * 81, 150, 81));
	fightingAnimation.addFrame(sf::IntRect(450, 2 * 81, 150, 81));
	fightingAnimation.addFrame(sf::IntRect(450, 3 * 81, 150, 81));
	fightingAnimation.addFrame(sf::IntRect(450, 2 * 81, 150, 81));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation deadAnimation;
	deadAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_nekomata_blue_trans));
	for (int i = 0; i < 1; i++)
	{
		deadAnimation.addFrame(sf::IntRect(600, i * 81, 150, 81));
	}

	addAnimation(GameObjectState::Dead, deadAnimation);

	setFrameTime(sf::seconds(0.08f));

	// initial values
	m_state = GameObjectState::Idle;
	m_isFacingRight = true;
	setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	playCurrentAnimation(true);
}

float NekomataEnemy::getConfiguredMaxVelocityY() const
{
	return 400.0f;
}

float NekomataEnemy::getConfiguredMaxVelocityX() const
{
	return 80.0f;
}

sf::Time NekomataEnemy::getConfiguredFightAnimationTime() const
{
	return sf::milliseconds(4 * 80);
}

float NekomataEnemy::getAggroRange() const
{
	return 500.f;
}

float NekomataEnemy::getApproachingDistance() const
{
	return 30.f;
}

float NekomataEnemy::getDistanceToAbyss() const
{
	return 100.f;
}
