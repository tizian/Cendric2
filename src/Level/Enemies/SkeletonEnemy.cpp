#include "Level/Enemies/SkeletonEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Skeleton, SkeletonEnemy)

void SkeletonEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 30 + 10;
}

void SkeletonEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 3 + 1;
}

SkeletonEnemy::SkeletonEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
}

void SkeletonEnemy::loadAttributes() {
	m_attributes.setHealth(100);
	m_attributes.resistanceIce = 0;
	m_attributes.resistanceFire = 30;
	m_attributes.resistanceShadow = 30;
	m_attributes.calculateAttributes();
}

void SkeletonEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 40;
	chopSpell.duration = sf::seconds(2.f);
	chopSpell.damagePerSecond = 10;
	chopSpell.activeDuration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(2000);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 70, 100);
	chopSpell.spellOffset = sf::Vector2f(10.f, 0.f);
	chopSpell.fightingTime = sf::milliseconds(8 * 50);
	
	m_spellManager->addSpell(chopSpell);

	m_spellManager->setCurrentSpell(0); // chop
}

void SkeletonEnemy::handleAttackInput() {
	if (m_enemyAttackingBehavior->distToTarget() < 180.f) {
		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
	}
}

void SkeletonEnemy::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 40.f, 135.f));
	setSpriteOffset(sf::Vector2f(-35.f, -15.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* walkingAnimation = new Animation(sf::seconds(0.1f));
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; i++) {
		walkingAnimation->addFrame(sf::IntRect(i * 150, 0, 150, 150));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(0, 0, 150, 150));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, 150, 150, 150));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* fightingAnimation = new Animation(sf::seconds(0.05f));
	fightingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; i++) {
		fightingAnimation->addFrame(sf::IntRect(i * 150, 300, 150, 150));
	}

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 2; i++) {
		deadAnimation->addFrame(sf::IntRect(i * 150, 450, 150, 150));
	}
	deadAnimation->setLooped(false);
	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

MovingBehavior* SkeletonEnemy::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyWalkingBehavior(this);
	}
	else {
		behavior = new AggressiveWalkingBehavior(this);
	}
	behavior->setDistanceToAbyss(100.f);
	behavior->setApproachingDistance(30.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityX(150.f);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* SkeletonEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(800.f);
	behavior->setAttackInput(std::bind(&SkeletonEnemy::handleAttackInput, this));
	return behavior;
}

int SkeletonEnemy::getMentalStrength() const {
	return 2;
}

std::string SkeletonEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_skeleton.png";
}

