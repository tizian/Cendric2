#include "Level/Enemies/SkeletonMageEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Skeleton_Mage, SkeletonMageEnemy)

void SkeletonMageEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 30 + 10;
}

void SkeletonMageEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 3 + 1;
}

SkeletonMageEnemy::SkeletonMageEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
}

void SkeletonMageEnemy::loadAttributes() {
	m_attributes.setHealth(100);
	m_attributes.resistanceFire = 30;
	m_attributes.resistanceShadow = 30;
	m_attributes.calculateAttributes();
}

void SkeletonMageEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 40;
	chopSpell.duration = sf::seconds(2.f);
	chopSpell.damagePerSecond = 10;
	chopSpell.activeDuration = sf::milliseconds(5 * 70);
	chopSpell.cooldown = sf::milliseconds(2000);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 40, 80);
	chopSpell.spellOffset = sf::Vector2f(10.f, 0.f);
	chopSpell.fightingTime = sf::milliseconds(5 * 70);
	chopSpell.castingTime = sf::milliseconds(3 * 70);
	
	m_spellManager->addSpell(chopSpell);

	m_spellManager->setCurrentSpell(0); // chop
}

void SkeletonMageEnemy::handleAttackInput() {
	if (m_enemyAttackingBehavior->distToTarget() < 180.f) {
		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
	}
}

void SkeletonMageEnemy::loadAnimation(int skinNr) {
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

	Animation* deadAnimation = new Animation(sf::milliseconds(50));
	deadAnimation->setSpriteSheet(tex);
	for (int i = 10; i < 15; ++i) {
		deadAnimation->addFrame(sf::IntRect(i * 120, skinNr * 120, 120, 120));
	}
	deadAnimation->setLooped(false);
	addAnimation(GameObjectState::Dead, deadAnimation);

	Animation* fightingStartAnimation = new Animation(sf::milliseconds(70));
	fightingStartAnimation->setSpriteSheet(tex);
	fightingStartAnimation->addFrame(sf::IntRect(15 * 120, skinNr * 120, 120, 120));
	fightingStartAnimation->addFrame(sf::IntRect(17 * 120, skinNr * 120, 120, 120));
	fightingStartAnimation->addFrame(sf::IntRect(16 * 120, skinNr * 120, 120, 120));

	addAnimation(GameObjectState::Casting, fightingStartAnimation);

	Animation* fightingAnimation = new Animation(sf::milliseconds(70));
	fightingAnimation->setSpriteSheet(tex);
	for (int i = 16; i < 21; ++i) {
		fightingAnimation->addFrame(sf::IntRect(i * 120, skinNr * 120, 120, 120));
	}
	
	addAnimation(GameObjectState::Fighting, fightingAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

MovingBehavior* SkeletonDefaultEnemy::createMovingBehavior(bool asAlly) {
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

AttackingBehavior* SkeletonMageEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(800.f);
	behavior->setAttackInput(std::bind(&SkeletonMageEnemy::handleAttackInput, this));
	return behavior;
}

int SkeletonMageEnemy::getMentalStrength() const {
	return 2;
}

std::string SkeletonMageEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_skeletonmage.png";
}

