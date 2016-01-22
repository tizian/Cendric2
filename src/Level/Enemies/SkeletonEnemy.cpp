#include "Level/Enemies/SkeletonEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/EnemyBehavior/WalkingBehavior.h"
#include "Level/EnemyBehavior/AggressiveBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Skeleton, SkeletonEnemy)

void SkeletonEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	if (gold != 0 || !loot.empty()) return;

	gold = rand() % 30 + 210;
}

SkeletonEnemy::SkeletonEnemy(Level* level, Screen* screen) :
	Enemy(level, screen),
	LevelMovableGameObject(level) {
	load(EnemyID::Skeleton);
}

void SkeletonEnemy::loadAttributes() {
	m_attributes.setHealth(400);
	m_attributes.resistanceIce = 30;
	m_attributes.resistanceFire = 30;
	m_attributes.resistanceShadow = 30;
	m_attributes.calculateAttributes();
}

void SkeletonEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 50;
	chopSpell.duration = sf::seconds(2.f);
	chopSpell.damagePerSecond = 10;
	chopSpell.activeDuration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(2000);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 70, 100);

	m_spellManager->addSpell(chopSpell);

	m_spellManager->setCurrentSpell(0); // chop
}

sf::Vector2f SkeletonEnemy::getConfiguredSpellOffset() const {
	return sf::Vector2f(10.f, 0.f);
}

void SkeletonEnemy::handleAttackInput() {
	if (m_attackingBehavior->distToTarget() < 180.f) {
		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
	}
}

void SkeletonEnemy::loadAnimation() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 40.f, 135.f));
	setSpriteOffset(sf::Vector2f(-35.f, -15.f));

	Animation walkingAnimation(sf::seconds(0.1f));
	walkingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_skeleton));
	for (int i = 0; i < 8; i++) {
		walkingAnimation.addFrame(sf::IntRect(i * 150, 0, 150, 150));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation jumpingAnimation;
	jumpingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_skeleton));
	jumpingAnimation.addFrame(sf::IntRect(0, 0, 150, 150));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_skeleton));
	idleAnimation.addFrame(sf::IntRect(00, 150, 150, 150));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation fightingAnimation(sf::seconds(0.05f));
	fightingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_skeleton));
	for (int i = 0; i < 8; i++) {
		fightingAnimation.addFrame(sf::IntRect(i * 150, 300, 150, 150));
	}

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation deadAnimation;
	deadAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_skeleton));
	for (int i = 0; i < 2; i++) {
		deadAnimation.addFrame(sf::IntRect(i * 150, 450, 150, 150));
	}
	deadAnimation.setLooped(false);
	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	m_state = GameObjectState::Idle;
	m_isFacingRight = true;
	setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	playCurrentAnimation(true);
}

MovingBehavior* SkeletonEnemy::createMovingBehavior() {
	WalkingBehavior* behavior = new WalkingBehavior(this);
	behavior->setJumpHeight(getConfiguredMaxVelocityYUp() * getConfiguredMaxVelocityYUp() / (2 * getConfiguredGravityAcceleration()));
	behavior->setDistanceToAbyss(100.f);
	behavior->setApproachingDistance(30.f);
	return behavior;
}

AttackingBehavior* SkeletonEnemy::createAttackingBehavior() {
	AggressiveBehavior* behavior = new AggressiveBehavior(this);
	behavior->setAggroRange(800.f);
	behavior->setAttackInput(std::bind(&SkeletonEnemy::handleAttackInput, this));
	return behavior;
}

float SkeletonEnemy::getMaxVelocityYUp() const {
	return 600.f;
}

float SkeletonEnemy::getMaxVelocityYDown() const {
	return 800.f;
}

float SkeletonEnemy::getMaxVelocityX() const {
	return 150.f;
}

sf::Time SkeletonEnemy::getConfiguredFightAnimationTime() const {
	return sf::milliseconds(8 * 50);
}

int SkeletonEnemy::getMentalStrength() const {
	return 2;
}

