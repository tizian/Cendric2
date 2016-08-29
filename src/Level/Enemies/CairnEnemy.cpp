#include "Level/Enemies/CairnEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Cairn, CairnEnemy)

void CairnEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 30 + 10;

	float xi = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	if (xi < 0.8f) {
		loot.insert({ "mi_stone", rand() % 3 + 1 });
	}
	else {
		loot.insert({ "mi_corrupt_stone", 1 });
	}
}

void CairnEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 3 + 1;
	float xi = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	if (xi < 0.9f) {
		loot.insert({ "mi_stone", 1 });
	}
	else {
		loot.insert({ "mi_corrupt_stone", 1 });
	}
}

CairnEnemy::CairnEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
}

void CairnEnemy::loadAttributes() {
	m_attributes.setHealth(100);
	m_attributes.resistanceIce = 15;
	m_attributes.resistanceFire = 10;
	m_attributes.resistanceShadow = 10;
	m_attributes.calculateAttributes();
}

void CairnEnemy::loadSpells() {
	SpellData fireBallSpell = SpellData::getSpellData(SpellID::FireBall);
	fireBallSpell.damage = 6;
	fireBallSpell.damagePerSecond = 0;
	fireBallSpell.duration = sf::seconds(2);
	fireBallSpell.cooldown = sf::milliseconds(3000);
	fireBallSpell.speed = 400.f;
	fireBallSpell.isDynamicTileEffect = false;
	fireBallSpell.damageType = DamageType::Ice;
	fireBallSpell.skinNr = 4;
	fireBallSpell.isBlocking = true;
	fireBallSpell.castingTime = sf::milliseconds(7 * 100);
	fireBallSpell.fightingTime = sf::Time::Zero;
	fireBallSpell.spellOffset = sf::Vector2f(10.f, 10.f);

	m_spellManager->addSpell(fireBallSpell);
	m_spellManager->setCurrentSpell(0);
}

void CairnEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < m_enemyAttackingBehavior->getAggroRange()) {
		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
	}
}

void CairnEnemy::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 30.f, 88.f));
	setSpriteOffset(sf::Vector2f(-37.f, -32.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	// Idle
	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(800, 0, 100, 120));
	addAnimation(GameObjectState::Idle, idleAnimation);

	// Walking
	Animation* walkingAnimation = new Animation(sf::seconds(0.12f));
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; i++) {
		walkingAnimation->addFrame(sf::IntRect(i * 100, 0, 100, 120));
	}
	addAnimation(GameObjectState::Walking, walkingAnimation);

	// Casting
	Animation* castingAnimation = new Animation(sf::seconds(0.12f));
	castingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 7; ++i) {
		castingAnimation->addFrame(sf::IntRect(1400 + i * 100, 0, 100, 120));
	}
	addAnimation(GameObjectState::Casting, castingAnimation);

	// Death
	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 6; ++i) {
		deadAnimation->addFrame(sf::IntRect(800 + i * 100, 0, 100, 120));
	}
	deadAnimation->setLooped(false);
	addAnimation(GameObjectState::Dead, deadAnimation);

	// Jump (not used)
	Animation* jumpingAnimation = new Animation(sf::seconds(0.12f));
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(800, 0, 100, 120));
	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

MovingBehavior* CairnEnemy::createMovingBehavior(bool asAlly) {
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

AttackingBehavior* CairnEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(800.f);
	behavior->setAttackInput(std::bind(&CairnEnemy::handleAttackInput, this));
	return behavior;
}

int CairnEnemy::getMentalStrength() const {
	return 2;
}

std::string CairnEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_cairn.png";
}

