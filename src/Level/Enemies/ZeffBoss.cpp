#include "Level/Enemies/ZeffBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"
#include "GlobalResource.h"

REGISTER_ENEMY(EnemyID::Boss_Zeff, ZeffBoss)

void ZeffBoss::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = 50;
	loot.insert({ "we_zeffssword", 1 });
	loot.insert({ "fo_ham", 2 });
}

ZeffBoss::~ZeffBoss() {
	delete m_ps;
}

void ZeffBoss::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	// nothing
}

ZeffBoss::ZeffBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
}

void ZeffBoss::loadAttributes() {
	m_attributes.setHealth(200);
	m_attributes.resistanceIce = -20;
	m_attributes.resistancePhysical = 50;
	m_attributes.critical = 20;
	m_attributes.calculateAttributes();
}

void ZeffBoss::loadSpells() {
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

void ZeffBoss::handleAttackInput() {
	if (m_enemyAttackingBehavior->distToTarget() < 150.f) {
		m_spellManager->setCurrentSpell(0); // spin
	}
	else {
		m_spellManager->setCurrentSpell(rand() % 2 + 1); // sword throw or boomerang
	}

	if (getCurrentTarget() != nullptr)
		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
}

void ZeffBoss::update(const sf::Time& frameTime) {
	Enemy::update(frameTime);
	if (m_isDead) {
		m_ps->update(frameTime);
		updateTime(m_fadingTime, frameTime);
		updateTime(m_particleTime, frameTime);
		if (m_particleTime == sf::Time::Zero) {
			m_ps->emitRate = 0;
		}
		setSpriteColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(m_fadingTime.asSeconds() / 2.f * 255.f)), sf::seconds(1000));
	}
}

void ZeffBoss::loadAnimation(int skinNr) {
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

	loadParticleSystem();
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
	behavior->setApproachingDistance(50.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(500.f);
	behavior->setMaxVelocityX(170.f);
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

void ZeffBoss::render(sf::RenderTarget& target) {
	Enemy::render(target);
	if (m_isDead) m_ps->render(target);
}

void ZeffBoss::setDead() {
	if (m_isDead) return;
	Enemy::setDead();
	updateParticleSystemPosition();
}

int ZeffBoss::getMentalStrength() const {
	return 2;
}

sf::Time ZeffBoss::getConfiguredWaitingTime() const {
	return sf::Time::Zero;
}

void ZeffBoss::updateParticleSystemPosition() {
	m_particleSpawner->center.x = getPosition().x + getBoundingBox()->width / 2.f;
	m_particleSpawner->center.y = getPosition().y + getBoundingBox()->height * (2.f / 3.f);
}

void ZeffBoss::loadParticleSystem() {
	m_ps = new particles::TextureParticleSystem(300, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_STAR));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 100.f;

	// Generators
	auto spawner = m_ps->addSpawner<particles::DiskSpawner>();
	spawner->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	spawner->radius = 20.f;
	m_particleSpawner = spawner;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 20.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 2.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(255, 255, 255, 255);
	colGen->maxStartCol = sf::Color(255, 255, 255, 255);
	colGen->minEndCol = sf::Color(255, 255, 255, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = -45.f;
	velGen->maxAngle = 45.f;
	velGen->minStartSpeed = 50.f;
	velGen->maxStartSpeed = 70.f;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 2.f;
	timeGen->maxTime = 3.f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}

std::string ZeffBoss::getSpritePath() const {
	return "res/assets/bosses/spritesheet_boss_zeff.png";
}

std::string ZeffBoss::getDeathSoundPath() const {
	return "res/sound/mob/cendric_death.ogg";
}


