#include "Level/Enemies/SkeletonWarriorEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Skeleton_Warrior, SkeletonWarriorEnemy)

void SkeletonWarriorEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 10 + 2;

	if (rand() % 100 > 80)
		loot.insert({ "fo_mediumhealingpotion", 1 });
}

void SkeletonWarriorEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 3 + 1;
}

SkeletonWarriorEnemy::SkeletonWarriorEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
}

void SkeletonWarriorEnemy::loadAttributes() {
	m_attributes.setHealth(100);
	m_attributes.resistanceFire = 50;
	m_attributes.resistanceShadow = 50;
	m_attributes.resistancePhysical = 50;
	m_attributes.critical = 10;
	m_attributes.calculateAttributes();
}

void SkeletonWarriorEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 40;
	chopSpell.duration = sf::seconds(2.f);
	chopSpell.damagePerSecond = 10;
	chopSpell.activeDuration = sf::milliseconds(300);
	chopSpell.cooldown = sf::seconds(2.f);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 70, 80);
	chopSpell.spellOffset = sf::Vector2f(0.f, 0.f);
	chopSpell.castingTime = sf::milliseconds(300);
	chopSpell.fightingTime = sf::milliseconds(300);
	
	m_spellManager->addSpell(chopSpell);

	m_spellManager->setCurrentSpell(0); // chop
}

void SkeletonWarriorEnemy::handleAttackInput() {
	if (m_enemyAttackingBehavior->distToTarget() < 140.f) {
		m_spellManager->executeCurrentSpell(getCurrentTarget());
	}
}

void SkeletonWarriorEnemy::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 30.f, 90.f));
	setSpriteOffset(sf::Vector2f(-45.f, -30.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* walkingAnimation = new Animation();
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; ++i) {
		walkingAnimation->addFrame(sf::IntRect(i * 120, skinNr * 120, 120, 120));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(8 * 120, skinNr * 120, 120, 120));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(9 * 120, skinNr * 120, 120, 120));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* deadAnimation = new Animation(sf::milliseconds(70));
	deadAnimation->setSpriteSheet(tex);
	for (int i = 10; i < 15; ++i) {
		deadAnimation->addFrame(sf::IntRect(i * 120, skinNr * 120, 120, 120));
	}
	deadAnimation->setLooped(false);
	addAnimation(GameObjectState::Dead, deadAnimation);

	Animation* fightingStartAnimation = new Animation();
	fightingStartAnimation->setSpriteSheet(tex);
	for (int i = 15; i < 18; ++i) {
		fightingStartAnimation->addFrame(sf::IntRect(i * 120, skinNr * 120, 120, 120));
	}

	addAnimation(GameObjectState::Casting, fightingStartAnimation);

	Animation* fightingAnimation = new Animation();
	fightingAnimation->setSpriteSheet(tex);
	for (int i = 18; i < 20; ++i) {
		fightingAnimation->addFrame(sf::IntRect(i * 120, skinNr * 120, 120, 120));
	}
	fightingAnimation->setLooped(false);
	
	addAnimation(GameObjectState::Fighting, fightingAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

MovingBehavior* SkeletonWarriorEnemy::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyWalkingBehavior(this);
	}
	else {
		behavior = new AggressiveWalkingBehavior(this);
	}
	behavior->setDistanceToAbyss(20.f);
	behavior->setApproachingDistance(30.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityX(180.f);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* SkeletonWarriorEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(700.f);
	behavior->setAttackInput(std::bind(&SkeletonWarriorEnemy::handleAttackInput, this));
	return behavior;
}

int SkeletonWarriorEnemy::getMentalStrength() const {
	return 2;
}

std::string SkeletonWarriorEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_skeletonwarrior.png";
}

