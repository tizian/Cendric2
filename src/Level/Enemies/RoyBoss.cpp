#include "Level/Enemies/RoyBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"
#include "GlobalResource.h"

REGISTER_ENEMY(EnemyID::Boss_Roy, RoyBoss)

RoyBoss::RoyBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen),
	Boss(level, screen) {
}

void RoyBoss::loadAttributes() {
	m_attributes.setHealth(200);
	m_attributes.resistanceIce = -20;
	m_attributes.resistancePhysical = 50;
	m_attributes.critical = 20;
	m_attributes.calculateAttributes();
}

void RoyBoss::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 20;
	chopSpell.activeDuration = sf::milliseconds(400);
	chopSpell.cooldown = sf::milliseconds(400);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 120, 100);
	chopSpell.spellOffset = sf::Vector2f(-60.f, 20.f);
	chopSpell.fightingTime = sf::milliseconds(400);
	
	m_spellManager->addSpell(chopSpell);

	SpellData projectile = SpellData::getSpellData(SpellID::ReturningProjectile);
	projectile.damage = 25;
	projectile.duration = sf::seconds(2.f);
	projectile.damagePerSecond = 6;
	projectile.cooldown = sf::milliseconds(7000);
	projectile.isBlocking = true;
	projectile.fightingTime = sf::seconds(10000);
	projectile.fightAnimation = GameObjectState::Fighting2;
	projectile.castingTime = sf::milliseconds(800);
	projectile.castingAnimation = GameObjectState::Casting;
	projectile.range = 600;
	projectile.speed = 500;
	
	m_spellManager->addSpell(projectile);

	SpellData boomerang = SpellData::getSpellData(SpellID::Boomerang);
	boomerang.damage = 20;
	boomerang.duration = sf::seconds(2.f);
	boomerang.damagePerSecond = 5;
	boomerang.cooldown = sf::milliseconds(8000);
	boomerang.isBlocking = true;
	boomerang.fightingTime = sf::seconds(10000);
	boomerang.fightAnimation = GameObjectState::Fighting2;
	boomerang.castingTime = sf::milliseconds(800);
	boomerang.castingAnimation = GameObjectState::Casting2;
	boomerang.range = 700;
	boomerang.speed = 500;

	m_spellManager->addSpell(boomerang);

	m_spellManager->setCurrentSpell(0); // chop
}

void RoyBoss::handleAttackInput() {
	if (m_enemyAttackingBehavior->distToTarget() < 150.f) {
		m_spellManager->setCurrentSpell(0); // spin
	}
	else {
		m_spellManager->setCurrentSpell(rand() % 2 + 1); // sword throw or boomerang
	}

	if (getCurrentTarget() != nullptr)
		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
}

void RoyBoss::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 60.f, 120.f));
	setSpriteOffset(sf::Vector2f(-30.f, -20.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* walkingAnimation = new Animation(sf::seconds(0.1f));
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; i++) {
		walkingAnimation->addFrame(sf::IntRect(i * 120, 0, 120, 140));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, 140, 120, 140));

	addAnimation(GameObjectState::Idle, idleAnimation);
	
	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(120, 140, 120, 140));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(2 * 120, 140, 120, 140));

	addAnimation(GameObjectState::Dead, deadAnimation);

	Animation* fightingAnimation = new Animation(sf::seconds(0.1f));
	fightingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 4; ++i) {
		fightingAnimation->addFrame(sf::IntRect(i * 120, 5 * 140, 120, 140));
	}

	addAnimation(GameObjectState::Fighting, fightingAnimation);
	
	Animation* fighting2Animation = new Animation(sf::seconds(0.1f));
	fighting2Animation->setSpriteSheet(tex);
	fighting2Animation->addFrame(sf::IntRect(0 * 120, 4 * 140, 120, 140));
	fighting2Animation->addFrame(sf::IntRect(1 * 120, 4 * 140, 120, 140));
	fighting2Animation->addFrame(sf::IntRect(2 * 120, 4 * 140, 120, 140));
	fighting2Animation->addFrame(sf::IntRect(1 * 120, 4 * 140, 120, 140));

	addAnimation(GameObjectState::Fighting2, fighting2Animation);

	Animation* castingAnimation = new Animation(sf::seconds(0.1f));
	castingAnimation->setSpriteSheet(tex);
	castingAnimation->setLooped(false);
	for (int i = 0; i < 4; ++i) {
		castingAnimation->addFrame(sf::IntRect(i * 120, 2 * 140, 120, 140));
	}

	addAnimation(GameObjectState::Casting, castingAnimation);

	Animation* casting2Animation = new Animation(sf::seconds(0.1f));
	casting2Animation->setSpriteSheet(tex);
	casting2Animation->setLooped(false);
	for (int i = 0; i < 4; ++i) {
		casting2Animation->addFrame(sf::IntRect(i * 120, 3 * 140, 120, 140));
	}

	addAnimation(GameObjectState::Casting2, casting2Animation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	loadDeathParticleSystem();
}

MovingBehavior* RoyBoss::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyWalkingBehavior(this);
	}
	else {
		behavior = new AggressiveWalkingBehavior(this);
	}
	behavior->setDistanceToAbyss(100.f);
	behavior->setApproachingDistance(50.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(500.f);
	behavior->setMaxVelocityX(170.f);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* RoyBoss::createAttackingBehavior(bool asAlly) {
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

sf::Time RoyBoss::getConfiguredWaitingTime() const {
	return sf::Time::Zero;
}

std::string RoyBoss::getSpritePath() const {
	return "res/assets/bosses/spritesheet_boss_mercenaries.png";
}
