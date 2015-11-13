#include "Enemies/FireRatEnemy.h"
#include "LevelMainCharacter.h"

FireRatEnemy::FireRatEnemy(Level* level, LevelMainCharacter* mainChar) : Enemy(level, mainChar, EnemyID::FireRat) {
	load();
	loadAttributes();
	loadSpells();
	m_jumpHeight = getConfiguredMaxVelocityY() * getConfiguredMaxVelocityY() / (2 * getConfiguredGravityAcceleration());
}

void FireRatEnemy::loadAttributes() {
	m_attributes.setHealth(70);
	m_attributes.resistanceIce = -20;
	m_attributes.calculateAttributes();

	m_immuneDamageTypes.push_back(DamageType::Fire);
}

void FireRatEnemy::loadSpells() {
	SpellBean chopSpell = SpellBean::getSpellBean(SpellID::Chop);
	chopSpell.damage = 15;
	chopSpell.duration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(1000);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 30, 30);

	SpellBean fireBallSpell = SpellBean::getSpellBean(SpellID::FireBall);
	fireBallSpell.damage = 10;
	fireBallSpell.cooldown = sf::milliseconds(3000);
	fireBallSpell.startVelocity = 200.f;

	m_spellManager->addSpell(chopSpell);
	m_spellManager->addSpell(fireBallSpell);
	m_spellManager->setCurrentSpell(0); // chop
}

sf::Vector2f FireRatEnemy::getConfiguredSpellOffset() const {
	return sf::Vector2f(-10.f, 0.f);
}

void FireRatEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (distToMainChar() < getAggroRange()) {
		m_spellManager->setCurrentSpell(1); // fire ball
		if (distToMainChar() < 100.f) {
			m_spellManager->setCurrentSpell(0); // chop
		}

		m_spellManager->executeCurrentSpell(m_mainChar->getCenter());
	}
}

void FireRatEnemy::handleMovementInput() {
	Enemy::handleMovementInput();
	// handle attack input

}

void FireRatEnemy::load() {
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

	Animation fightingAnimation(sf::seconds(0.08f));
	fightingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_firerat));
	fightingAnimation.addFrame(sf::IntRect(200, 0, 50, 50));
	fightingAnimation.addFrame(sf::IntRect(250, 0, 50, 50));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation deadAnimation;
	deadAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_firerat));
	deadAnimation.addFrame(sf::IntRect(350, 0, 50, 50));

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	m_state = GameObjectState::Idle;
	m_isFacingRight = true;
	setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	playCurrentAnimation(true);
}

float FireRatEnemy::getConfiguredMaxVelocityY() const {
	return 400.0f;
}

float FireRatEnemy::getConfiguredMaxVelocityX() const {
	return 50.0f;
}

sf::Time FireRatEnemy::getConfiguredFightAnimationTime() const {
	return sf::milliseconds(4 * 80);
}

float FireRatEnemy::getAggroRange() const {
	return 500.f;
}

float FireRatEnemy::getApproachingDistance() const {
	return 10.f;
}
