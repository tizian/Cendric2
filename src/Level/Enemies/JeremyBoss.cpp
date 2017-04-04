#include "Level/Enemies/JeremyBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "Registrar.h"
#include "GlobalResource.h"

REGISTER_ENEMY(EnemyID::Boss_Jeremy, JeremyBoss)

JeremyBoss::JeremyBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {

	m_isAlwaysUpdate = true;
	m_isBoss = true;
}

void JeremyBoss::update(const sf::Time& frameTime) {
	m_ps->flipOffsetX(!m_movingBehavior->isFacingRight());
	m_light->flipOffsetX(!m_movingBehavior->isFacingRight());
	Enemy::update(frameTime);
}

void JeremyBoss::loadAttributes() {
	m_attributes.setHealth(300);
	m_attributes.resistanceIce = -20;
	m_attributes.resistanceLight = -20;
	m_attributes.resistanceFire = 50;
	m_attributes.resistanceShadow = 10000;
	m_attributes.critical = 0;
	m_attributes.calculateAttributes();
}

void JeremyBoss::loadSpells() {
	// this is the shadow fireball
	SpellData fireball = SpellData::getSpellData(SpellID::FireBall);
	fireball.damageType = DamageType::Shadow;
	fireball.skinNr = 2;
	fireball.count = 2;
	fireball.damage = 10;
	fireball.damagePerSecond = 2;
	fireball.isStunning = true;
	fireball.duration = sf::seconds(2.f);
	fireball.cooldown = sf::seconds(5.f);
	fireball.fightingTime = sf::seconds(0.f);
	fireball.castingTime = sf::seconds(0.f);
	fireball.speed = 400;

	m_spellManager->addSpell(fireball);

	// the trap
	SpellData trap = SpellData::getSpellData(SpellID::ShadowTrap);
	trap.damageType = DamageType::Shadow;
	trap.damage = 25;
	trap.damagePerSecond = 6;
	trap.count = 3;
	trap.cooldown = sf::seconds(5.f);
	trap.isBlocking = true;
	trap.isStunning = true;
	trap.duration = sf::seconds(2.f);
	trap.activeDuration = sf::seconds(10.f);
	trap.fightingTime = sf::seconds(1.f);
	trap.fightAnimation = GameObjectState::Fighting;
	trap.castingTime = sf::seconds(0.6f);
	trap.castingAnimation = GameObjectState::Casting;

	m_spellManager->addSpell(trap);

	// the shadow step ninja icy ambush
	SpellData icyAmbush = SpellData::getSpellData(SpellID::IcyAmbush);
	icyAmbush.damageType = DamageType::Shadow;
	icyAmbush.skinNr = 1;
	icyAmbush.damage = 40;
	icyAmbush.damagePerSecond = 6;
	icyAmbush.cooldown = sf::seconds(10.f);
	icyAmbush.isBlocking = true;
	icyAmbush.isStunning = true;
	icyAmbush.duration = sf::seconds(2.f);
	icyAmbush.activeDuration = sf::seconds(10.f);
	icyAmbush.fightingTime = sf::seconds(0.f);
	icyAmbush.castingTime = sf::seconds(2.f);
	icyAmbush.castingAnimation = GameObjectState::Casting2;
	icyAmbush.speed = 500.f;
	icyAmbush.range = 500.f;

	m_spellManager->addSpell(icyAmbush);

	m_spellManager->setCurrentSpell(0); // stun
	m_spellManager->setGlobalCooldown(sf::seconds(3.f));
}

void JeremyBoss::handleAttackInput() {
	if (m_enemyAttackingBehavior->distToTarget() < 150.f) {
		m_spellManager->setCurrentSpell(1); // trap
	}
	else {
		m_spellManager->setCurrentSpell(rand() % 2 == 0 ? 2 : 0);
	}

	if (getCurrentTarget() != nullptr)
		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
}

void JeremyBoss::loadAnimation(int skinNr) {
	int size = 120;

	setBoundingBox(sf::FloatRect(0.f, 0.f, 30.f, 90.f));
	setSpriteOffset(sf::Vector2f(-45.f, -30.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* walkingAnimation = new Animation(sf::seconds(0.1f));
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; i++) {
		walkingAnimation->addFrame(sf::IntRect(i * size, 0, size, size));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(8 * size, 0, size, size));

	addAnimation(GameObjectState::Idle, idleAnimation);
	
	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(9 * size, 0, size, size));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(10 * size, 0, size, size));
	deadAnimation->setLooped(false);

	addAnimation(GameObjectState::Dead, deadAnimation);

	Animation* castingAnimation = new Animation(sf::seconds(0.2f));
	castingAnimation->setSpriteSheet(tex);
	for (int i = 16; i < 19; ++i) {
		castingAnimation->addFrame(sf::IntRect(i * size, 0, size, size));
	}
	castingAnimation->setLooped(false);

	addAnimation(GameObjectState::Casting, castingAnimation);

	Animation* fightingAnimation = new Animation(sf::seconds(0.1f));
	fightingAnimation->setSpriteSheet(tex);
	fightingAnimation->addFrame(sf::IntRect(19 * size, 0, size, size));
	fightingAnimation->setLooped(false);

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* casting2Animation = new Animation(sf::seconds(0.2f));
	casting2Animation->setSpriteSheet(tex);
	for (int i = 11; i < 16; ++i) {
		casting2Animation->addFrame(sf::IntRect(i * size, 0, size, size));
	}
	casting2Animation->setLooped(false);

	addAnimation(GameObjectState::Casting2, casting2Animation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	loadParticleSystem();
}

MovingBehavior* JeremyBoss::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	behavior = new AggressiveWalkingBehavior(this);
	behavior->setDistanceToAbyss(100.f);
	behavior->setApproachingDistance(100.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(500.f);
	behavior->setMaxVelocityX(200.f);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* JeremyBoss::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	behavior = new AggressiveBehavior(this);
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&JeremyBoss::handleAttackInput, this));
	return behavior;
}

void JeremyBoss::loadParticleSystem() {
	// add particles
	ParticleComponentData data;
	data.emitRate = 30.f;
	data.particleCount = 60;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_BLOB;

	auto spawner = new particles::DiskSpawner();
	spawner->radius = 5.f;
	data.spawner = spawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 5.f;
	sizeGen->maxStartSize = 10.f;
	sizeGen->minEndSize = 2.f;
	sizeGen->maxEndSize = 5.f;
	data.sizeGen = sizeGen;
	
	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = sf::Color(113, 128, 186, 100);
	colGen->maxStartCol = sf::Color(230, 230, 255, 255);
	colGen->minEndCol = sf::Color(69, 95, 122, 0);
	colGen->maxEndCol = sf::Color(170, 179, 239, 0);
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = -45.f;
	velGen->maxAngle = 45.f;
	velGen->minStartSpeed = 20.f;
	velGen->maxStartSpeed = 40.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 1.f;
	timeGen->maxTime = 2.f;
	data.timeGen = timeGen;

	m_ps = new ParticleComponent(data, this);
	m_ps->setOffset(sf::Vector2f(30.f, 10.f));
	addComponent(m_ps);

	// add light
	LightData lightData(sf::Vector2f(30.f, 0.f), sf::Vector2f(100.f, 100.f), 0.9f);
	m_light = new LightComponent(lightData, this);
	addComponent(m_light);
}

sf::Time JeremyBoss::getConfiguredWaitingTime() const {
	return sf::Time::Zero;
}

float JeremyBoss::getConfiguredDistanceToHPBar() const {
	return 30.f;
}

std::string JeremyBoss::getSpritePath() const {
	return "res/assets/bosses/spritesheet_boss_jeremy.png";
}
