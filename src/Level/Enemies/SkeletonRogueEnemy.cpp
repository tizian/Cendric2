#include "Level/Enemies/SkeletonRogueEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Skeleton_Rogue, SkeletonRogueEnemy)

void SkeletonRogueEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 10 + 2;

	if (rand() % 100 > 80)
		loot.insert({ "fo_lesserhealingpotion", 1 });
}

void SkeletonRogueEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 3 + 1;
}

SkeletonRogueEnemy::SkeletonRogueEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
}

void SkeletonRogueEnemy::loadAttributes() {
	m_attributes.setHealth(80);
	m_attributes.resistanceFire = 30;
	m_attributes.resistanceShadow = 30;
	m_attributes.critical = 33;
	m_attributes.calculateAttributes();
}

void SkeletonRogueEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 20;
	chopSpell.duration = sf::seconds(2.f);
	chopSpell.damagePerSecond = 10;
	chopSpell.activeDuration = sf::milliseconds(4 * 70);
	chopSpell.cooldown = sf::milliseconds(1000);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 40, 90);
	chopSpell.spellOffset = sf::Vector2f(10.f, -10.f);
	chopSpell.fightingTime = sf::milliseconds(4 * 70);
	chopSpell.castingTime = sf::milliseconds(4 * 70);
	
	m_spellManager->addSpell(chopSpell);

	m_spellManager->setCurrentSpell(0); // chop
}

void SkeletonRogueEnemy::handleAttackInput() {
	if (m_enemyAttackingBehavior->distToTarget() < 120.f) {
		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
	}
}

void SkeletonRogueEnemy::loadAnimation(int skinNr) {
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

	Animation* fightingStartAnimation = new Animation(sf::milliseconds(70));
	fightingStartAnimation->setSpriteSheet(tex);
	fightingStartAnimation->addFrame(sf::IntRect(15 * 120, skinNr * 120, 120, 120));
	fightingStartAnimation->addFrame(sf::IntRect(16 * 120, skinNr * 120, 120, 120));
	fightingStartAnimation->addFrame(sf::IntRect(17 * 120, skinNr * 120, 120, 120));
	fightingStartAnimation->addFrame(sf::IntRect(18* 120, skinNr * 120, 120, 120));

	addAnimation(GameObjectState::Casting, fightingStartAnimation);

	Animation* fightingAnimation = new Animation(sf::milliseconds(70));
	fightingAnimation->setSpriteSheet(tex);
	fightingAnimation->addFrame(sf::IntRect(18 * 120, skinNr * 120, 120, 120));
	fightingAnimation->addFrame(sf::IntRect(17 * 120, skinNr * 120, 120, 120));
	fightingAnimation->addFrame(sf::IntRect(19 * 120, skinNr * 120, 120, 120));
	fightingAnimation->addFrame(sf::IntRect(20 * 120, skinNr * 120, 120, 120));
	
	addAnimation(GameObjectState::Fighting, fightingAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

MovingBehavior* SkeletonRogueEnemy::createMovingBehavior(bool asAlly) {
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
	behavior->setMaxVelocityX(200.f);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* SkeletonRogueEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(800.f);
	behavior->setAttackInput(std::bind(&SkeletonRogueEnemy::handleAttackInput, this));
	return behavior;
}

int SkeletonRogueEnemy::getMentalStrength() const {
	return 2;
}

std::string SkeletonRogueEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_skeletonrogue.png";
}

