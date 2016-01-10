#include "Enemies/RatEnemy.h"
#include "Level/LevelMainCharacter.h"

RatEnemy::RatEnemy(Level* level, LevelMainCharacter* mainChar) : Enemy(level, mainChar, EnemyID::Rat) {
	load();
	loadAttributes();
	loadSpells();
	m_jumpHeight = getConfiguredMaxVelocityYUp() * getConfiguredMaxVelocityYUp() / (2 * getConfiguredGravityAcceleration());
}

void RatEnemy::loadAttributes() {
	m_attributes.setHealth(40);
	m_attributes.resistancePhysical = 10;
	m_attributes.calculateAttributes();
}

void RatEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.duration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(1000);
	chopSpell.damage = 2;
	chopSpell.boundingBox = sf::FloatRect(10, 0, 30, 30);

	m_spellManager->addSpell(chopSpell);
	m_spellManager->setCurrentSpell(0);
}

sf::Vector2f RatEnemy::getConfiguredSpellOffset() const {
	return sf::Vector2f(-10.f, 0.f);
}

void RatEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (distToMainChar() < 100.f) {
		m_spellManager->executeCurrentSpell(m_mainChar->getCenter());
	}
}

float RatEnemy::getAggroRange() const {
	return 300.f;
}

float RatEnemy::getDistanceToAbyss() const {
	return 20.f;
}

bool RatEnemy::getFleeCondition() const {
	// what a cowardly rat
	return m_attributes.currentHealthPoints < m_attributes.maxHealthPoints / 3;
}

float RatEnemy::getApproachingDistance() const {
	return 10.f;
}

void RatEnemy::handleMovementInput() {
	Enemy::handleMovementInput();
}

void RatEnemy::load() {
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

	Animation fightingAnimation(sf::seconds(0.08f));
	fightingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_rat));
	fightingAnimation.addFrame(sf::IntRect(200, 0, 50, 50));
	fightingAnimation.addFrame(sf::IntRect(250, 0, 50, 50));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation deadAnimation;
	deadAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_rat));
	deadAnimation.addFrame(sf::IntRect(300, 0, 50, 50));

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	m_state = GameObjectState::Idle;
	m_isFacingRight = true;
	setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	playCurrentAnimation(true);
}

sf::Time RatEnemy::getConfiguredFightAnimationTime() const {
	return sf::milliseconds(4 * 80);
}

float RatEnemy::getMaxVelocityYUp() const {
	return 400.f;
}

float RatEnemy::getMaxVelocityYDown() const {
	return 400.f;
}

float RatEnemy::getMaxVelocityX() const {
	return 100.f;
}
