#include "Level/Enemies/FireRatEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::FireRat, FireRatEnemy)

void FireRatEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	if (gold != 0 || !loot.empty()) return;

	loot.insert({ "fo_rawmeat", 1 });
	loot.insert({ "fo_cheese", 1 });
	gold = rand() % 4 + 1;
}

FireRatEnemy::FireRatEnemy(Level* level, Screen* screen, bool isControlled) :
	WalkingEnemy(level, screen, isControlled),
	Enemy(level, screen, isControlled),
	LevelMovableGameObject(level) {
	m_id = EnemyID::FireRat;
	load();
	loadAttributes();
	loadSpells();
	m_jumpHeight = getConfiguredMaxVelocityYUp() * getConfiguredMaxVelocityYUp() / (2 * getConfiguredGravityAcceleration());
}

void FireRatEnemy::loadAttributes() {
	m_attributes.setHealth(70);
	m_attributes.resistanceIce = -20;
	m_attributes.calculateAttributes();

	m_immuneDamageTypes.push_back(DamageType::Fire);
}

void FireRatEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 15;
	chopSpell.activeDuration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(1000);
	chopSpell.boundingBox = sf::FloatRect(10, 0, 30, 30);

	SpellData fireBallSpell = SpellData::getSpellData(SpellID::FireBall);
	fireBallSpell.damage = 10;
	fireBallSpell.cooldown = sf::milliseconds(3000);
	fireBallSpell.speed = 200.f;

	m_spellManager->addSpell(chopSpell);
	m_spellManager->addSpell(fireBallSpell);
	m_spellManager->setCurrentSpell(0); // chop
}

sf::Vector2f FireRatEnemy::getConfiguredSpellOffset() const {
	return sf::Vector2f(-10.f, 0.f);
}

void FireRatEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (m_currentTarget == nullptr) return;
	if (distToTarget() < getAggroRange()) {
		m_spellManager->setCurrentSpell(1); // fire ball
		if (distToTarget() < 50.f) {
			m_spellManager->setCurrentSpell(0); // chop
		}

		m_spellManager->executeCurrentSpell(m_currentTarget->getCenter());
	}
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

float FireRatEnemy::getMaxVelocityYUp() const {
	return 400.f;
}

float FireRatEnemy::getMaxVelocityYDown() const {
	return 400.f;
}

float FireRatEnemy::getMaxVelocityX() const {
	return 50.f;
}

sf::Time FireRatEnemy::getConfiguredFightAnimationTime() const {
	return sf::milliseconds(4 * 80);
}

float FireRatEnemy::getAggroRange() const {
	return 500.f;
}

int FireRatEnemy::getMentalStrength() const {
	return 1;
}

float FireRatEnemy::getApproachingDistance() const {
	return 10.f;
}
