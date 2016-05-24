#include "Level/Enemies/StonemanEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Stoneman, StonemanEnemy)

void StonemanEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 30 + 10;

	float xi = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	if (xi < 0.8f) {
		loot.insert({ "mi_stone", rand() % 3 + 1 });
	}
	else {
		loot.insert({ "mi_corrupt_stone", 1 });
	}
}

void StonemanEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 3 + 1;
	float xi = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	if (xi < 0.9f) {
		loot.insert({ "mi_stone", 1 });
	}
	else {
		loot.insert({ "mi_corrupt_stone", 1 });
	}
}

StonemanEnemy::StonemanEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
	load(EnemyID::Stoneman);
}

void StonemanEnemy::loadAttributes() {
	m_attributes.setHealth(100);
	m_attributes.resistanceIce = 15;
	m_attributes.resistanceFire = 10;
	m_attributes.resistanceShadow = 10;
	m_attributes.calculateAttributes();
}

void StonemanEnemy::loadSpells() {
	SpellData fireBallSpell = SpellData::getSpellData(SpellID::FireBall);
	fireBallSpell.damage = 6;
	fireBallSpell.damagePerSecond = 0;
	fireBallSpell.duration = sf::seconds(2);
	fireBallSpell.cooldown = sf::milliseconds(3000);
	fireBallSpell.speed = 400.f;
	fireBallSpell.isDynamicTileEffect = false;
	fireBallSpell.damageType = DamageType::Ice;
	fireBallSpell.skinNr = 1;
	fireBallSpell.isBlocking = true;
	fireBallSpell.castingTime = sf::milliseconds(7 * 100);
	fireBallSpell.fightingTime = sf::Time::Zero;

	m_spellManager->addSpell(fireBallSpell);
	m_spellManager->setCurrentSpell(0);
}

sf::Vector2f StonemanEnemy::getConfiguredSpellOffset() const {
	return sf::Vector2f(19.f, 14.f);
}

void StonemanEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < m_enemyAttackingBehavior->getAggroRange()) {
		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
	}
}

void StonemanEnemy::loadAnimation() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 30.f, 88.f));
	setSpriteOffset(sf::Vector2f(-37.f, -32.f));

	// Idle
	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_stoneman));
	idleAnimation->addFrame(sf::IntRect(800, 0, 100, 120));
	addAnimation(GameObjectState::Idle, idleAnimation);

	// Walking
	Animation* walkingAnimation = new Animation(sf::seconds(0.12f));
	walkingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_stoneman));
	for (int i = 0; i < 8; i++) {
		walkingAnimation->addFrame(sf::IntRect(i * 100, 0, 100, 120));
	}
	addAnimation(GameObjectState::Walking, walkingAnimation);

	// Casting
	Animation* castingAnimation = new Animation(sf::seconds(0.12f));
	castingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_stoneman));
	for (int i = 0; i < 7; ++i) {
		castingAnimation->addFrame(sf::IntRect(1400 + i * 100, 0, 100, 120));
	}
	addAnimation(GameObjectState::Casting, castingAnimation);

	// Death
	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_stoneman));
	for (int i = 0; i < 6; ++i) {
		deadAnimation->addFrame(sf::IntRect(800 + i * 100, 0, 100, 120));
	}
	deadAnimation->setLooped(false);
	addAnimation(GameObjectState::Dead, deadAnimation);

	// Jump (not used)
	Animation* jumpingAnimation = new Animation(sf::seconds(0.12f));
	jumpingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_stoneman));
	jumpingAnimation->addFrame(sf::IntRect(800, 0, 100, 120));
	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

MovingBehavior* StonemanEnemy::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyWalkingBehavior(this);
	}
	else {
		behavior = new AggressiveWalkingBehavior(this);
	}
	behavior->setDistanceToAbyss(30.f);
	behavior->setApproachingDistance(30.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(0.f);
	behavior->setMaxVelocityX(80.f);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* StonemanEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(800.f);
	behavior->setAttackInput(std::bind(&StonemanEnemy::handleAttackInput, this));
	return behavior;
}

int StonemanEnemy::getMentalStrength() const {
	return 2;
}

