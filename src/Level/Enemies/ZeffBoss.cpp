#include "Level/Enemies/ZeffBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Boss_Zeff, ZeffBoss)

void ZeffBoss::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 30 + 10;
}

void ZeffBoss::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 3 + 1;
}

ZeffBoss::ZeffBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
	load(EnemyID::Boss_Zeff);
}

void ZeffBoss::loadAttributes() {
	m_attributes.setHealth(100);
	m_attributes.resistanceIce = 0;
	m_attributes.resistanceFire = 30;
	m_attributes.resistanceShadow = 30;
	m_attributes.calculateAttributes();
}

void ZeffBoss::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 20;
	chopSpell.activeDuration = sf::milliseconds(400);
	chopSpell.cooldown = sf::milliseconds(400);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 120, 100);
	chopSpell.fightingTime = sf::milliseconds(400);

	m_spellManager->addSpell(chopSpell);

	SpellData projectile = SpellData::getSpellData(SpellID::ReturningProjectile);
	projectile.damage = 10;
	projectile.duration = sf::seconds(2.f);
	projectile.damagePerSecond = 2;
	projectile.cooldown = sf::milliseconds(2000);
	projectile.fightingTime = sf::seconds(10000);
	projectile.isBlocking = true;
	projectile.fightAnimation = GameObjectState::Fighting2;
	projectile.castingTime = sf::milliseconds(800);
	projectile.castingAnimation = GameObjectState::Casting;
	projectile.range = 300;
	
	m_spellManager->addSpell(projectile);

	m_spellManager->setCurrentSpell(0); // chop
}

sf::Vector2f ZeffBoss::getConfiguredSpellOffset() const {
	return sf::Vector2f(10.f, 0.f);
}

void ZeffBoss::handleAttackInput() {
	if (m_enemyAttackingBehavior->distToTarget() < 150.f) {
		m_spellManager->setCurrentSpell(0); // spin
	}
	m_spellManager->setCurrentSpell(1);
	m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
}

void ZeffBoss::loadAnimation() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 60.f, 120.f));
	setSpriteOffset(sf::Vector2f(-30.f, -20.f));

	Animation* walkingAnimation = new Animation(sf::seconds(0.1f));
	walkingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_boss_zeff));
	for (int i = 0; i < 8; i++) {
		walkingAnimation->addFrame(sf::IntRect(i * 120, 0, 120, 140));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_boss_zeff));
	idleAnimation->addFrame(sf::IntRect(0, 140, 120, 140));

	addAnimation(GameObjectState::Idle, idleAnimation);
	
	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_boss_zeff));
	jumpingAnimation->addFrame(sf::IntRect(120, 140, 120, 140));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_boss_zeff));
	deadAnimation->addFrame(sf::IntRect(2 * 120, 140, 120, 140));

	addAnimation(GameObjectState::Dead, deadAnimation);

	Animation* fightingAnimation = new Animation(sf::seconds(0.1f));
	fightingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_boss_zeff));
	for (int i = 0; i < 4; ++i) {
		fightingAnimation->addFrame(sf::IntRect(i * 120, 5 * 140, 120, 140));
	}

	addAnimation(GameObjectState::Fighting, fightingAnimation);
	
	Animation* fighting2Animation = new Animation(sf::seconds(0.1f));
	fighting2Animation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_boss_zeff));
	fighting2Animation->addFrame(sf::IntRect(4 * 120, 0 * 140, 120, 140));
	fighting2Animation->addFrame(sf::IntRect(4 * 120, 1 * 140, 120, 140));
	fighting2Animation->addFrame(sf::IntRect(4 * 120, 2 * 140, 120, 140));
	fighting2Animation->addFrame(sf::IntRect(4 * 120, 1 * 140, 120, 140));

	addAnimation(GameObjectState::Fighting2, fighting2Animation);

	Animation* castingAnimation = new Animation(sf::seconds(0.1f));
	castingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_boss_zeff));
	castingAnimation->setLooped(false);
	for (int i = 0; i < 4; ++i) {
		castingAnimation->addFrame(sf::IntRect(i * 120, 2 * 140, 120, 140));
	}

	addAnimation(GameObjectState::Casting, castingAnimation);

	Animation* casting2Animation = new Animation(sf::seconds(0.1f));
	casting2Animation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_boss_zeff));
	casting2Animation->setLooped(false);
	for (int i = 0; i < 4; ++i) {
		casting2Animation->addFrame(sf::IntRect(i * 120, 3 * 140, 120, 140));
	}

	addAnimation(GameObjectState::Casting2, casting2Animation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

MovingBehavior* ZeffBoss::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyWalkingBehavior(this);
	}
	else {
		behavior = new AggressiveWalkingBehavior(this);
	}
	behavior->setDistanceToAbyss(100.f);
	behavior->setApproachingDistance(100.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(500.f);
	behavior->setMaxVelocityX(150.f);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* ZeffBoss::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&ZeffBoss::handleAttackInput, this));
	return behavior;
}

int ZeffBoss::getMentalStrength() const {
	return 2;
}

