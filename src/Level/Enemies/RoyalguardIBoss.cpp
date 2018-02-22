#include "Level/Enemies/RoyalguardIBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/RoyalguardIBossMovingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "World/CustomParticleUpdaters.h"
#include "Level/DynamicTiles/ParticleTile.h"
#include "Screens/LevelScreen.h"
#include "Registrar.h"
#include "GlobalResource.h"

REGISTER_ENEMY(EnemyID::Boss_Royalguard_I, RoyalguardIBoss)

const sf::Time RoyalguardIBoss::FIRE_TIME = sf::seconds(10.f);
const std::string RoyalguardIBoss::FIRE_SOUND = "res/sound/spell/fire_explosion.ogg";

RoyalguardIBoss::RoyalguardIBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen),
	RoyalguardBoss(level, screen) {
	loadBoss();
	g_resourceManager->loadSoundbuffer(FIRE_SOUND, ResourceType::Level);
}

void RoyalguardIBoss::loadAttributes() {
	m_attributes.setHealth(400);
	m_attributes.resistanceIce = -20;
	m_attributes.resistancePhysical = 100;
	m_attributes.resistanceShadow = 100;
	m_attributes.resistanceLight = 100;
	m_attributes.resistanceFire = 10000;
	m_attributes.critical = 0;
	m_attributes.calculateAttributes();
}

void RoyalguardIBoss::loadSpells() {
	// fireball
	SpellData fireball = SpellData::getSpellData(SpellID::FireBall);
	fireball.damage = 30;
	fireball.damagePerSecond = 1;
	fireball.cooldown = sf::seconds(1.f);
	fireball.fightingTime = sf::Time::Zero;
	fireball.isBlocking = true;

	m_spellManager->addSpell(fireball);

	m_spellManager->setCurrentSpell(0);
}

void RoyalguardIBoss::handleAttackInput() {
	// nop
}

void RoyalguardIBoss::addDamage(int damage, DamageType damageType, bool overTime, bool critical) {
	if (damageType == DamageType::Fire) {
		addHeal(damage, overTime, critical);
		return;
	}

	RoyalguardBoss::addDamage(damage, damageType, overTime, critical);
}

void RoyalguardIBoss::updateBossState(const sf::Time& frameTime) {
	if (m_isDead) return;
	updateTime(m_stateTime, frameTime);
	updateTime(m_fireTime, frameTime);
	
	if (m_fireSoundTime > sf::Time::Zero) {
		updateTime(m_fireSoundTime, frameTime);
		if (m_fireSoundTime == sf::Time::Zero) {
			g_resourceManager->playSound(FIRE_SOUND, false, 0.6f);
		}
	}

	if (m_stateTime > sf::Time::Zero) return;
	switch (m_bossState)
	{
	default:
	case RoyalguardBossState::Waiting:
		if (m_fireTime == sf::Time::Zero) {
			m_isTopFire = !m_isTopFire;
			m_bossState = m_isTopFire ? RoyalguardBossState::TopFire : RoyalguardBossState::BottomFire;
			m_stateTime = sf::seconds(2.f);
			m_fireLinePc->reset();
			m_fireLinePc->setVisible(true);
			m_lineSpawner->point1 = getPosition() + sf::Vector2f(m_boundingBox.width * 0.5f, -20.f);
			m_lineSpawner->point2 = m_isTopFire ? RoyalguardFire::FIRE_POS_TOP : RoyalguardFire::FIRE_POS_BOT;
			auto dir = m_lineSpawner->point2 - m_lineSpawner->point1;
			m_lineVelGen->minAngle = radToDeg(std::atan2(dir.y, dir.x)) + 90.f;
			m_lineVelGen->maxAngle = m_lineVelGen->minAngle;
			m_fireSoundTime = sf::seconds(1.4f);
		}
		else {
			m_bossState = RoyalguardBossState::FireballStart;
			m_stateTime = sf::seconds(2.f);
			m_weaponOffset = sf::Vector2f(34.f, -5.f);
			m_isWeaponVisible = true;
			m_weaponRotateType = WeaponRotateType::ToMainChar;
		}
		m_fireLinePc->reset();
		m_firePc->setVisible(true);
		break;
	case RoyalguardBossState::FireballStart:
		m_bossState = RoyalguardBossState::Waiting;
		m_spellManager->executeCurrentSpell(m_mainChar);
		m_stateTime = sf::seconds(1.f);
		m_isWeaponVisible = false;
		m_firePc->setVisible(false);
		break;
	case RoyalguardBossState::TopFire:
	case RoyalguardBossState::BottomFire: {
		m_bossState = RoyalguardBossState::Waiting;
		m_stateTime = sf::seconds(1.f);
		RoyalguardFire* fire = new RoyalguardFire(m_isTopFire, m_mainChar);
		m_screen->addObject(fire);
		m_fireTime = FIRE_TIME;
		m_fireLinePc->setVisible(false);
		break;
	}
	case RoyalguardBossState::Healing:
		m_isWeaponVisible = false;
		m_bossState = RoyalguardBossState::Waiting;
		m_stateTime = sf::seconds(1.f);
		m_healingPc->setVisible(false);
		m_other->revive();
		break;
	}
}

void RoyalguardIBoss::loadAnimation(int skinNr) {
	int width = 120;
	int wideWidth = 240;
	int height = 150;

	setBoundingBox(sf::FloatRect(0.f, 0.f, 30.f, 90.f));
	setSpriteOffset(sf::Vector2f(-45.f, -60.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* walkingAnimation = new Animation(sf::seconds(0.1f));
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; i++) {
		walkingAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(8 * width, 0, width, height));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(9 * width, 0, width, height));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(10 * width, 0, width, height));
	deadAnimation->setLooped(false);

	addAnimation(GameObjectState::Dead, deadAnimation);

	// chop down
	Animation* casting1Animation = new Animation(sf::seconds(0.1f));
	casting1Animation->setSpriteSheet(tex);
	casting1Animation->addFrame(sf::IntRect(11 * width, 0, width, height));
	casting1Animation->addFrame(sf::IntRect(12 * width, 0, width, height));
	casting1Animation->addFrame(sf::IntRect(13 * width, 0, wideWidth, height));
	casting1Animation->addFrame(sf::IntRect(15 * width, 0, wideWidth, height));
	casting1Animation->addFrame(sf::IntRect(17 * width, 0, wideWidth, height));
	casting1Animation->setLooped(false);

	addAnimation(GameObjectState::Casting, casting1Animation);

	// chop up
	Animation* casting2Animation = new Animation(sf::seconds(0.1f));
	casting2Animation->setSpriteSheet(tex);
	casting2Animation->addFrame(sf::IntRect(17 * width, 0, wideWidth, height));
	casting2Animation->addFrame(sf::IntRect(15 * width, 0, wideWidth, height));
	casting2Animation->addFrame(sf::IntRect(13 * width, 0, wideWidth, height));
	casting2Animation->addFrame(sf::IntRect(12 * width, 0, width, height));
	casting2Animation->addFrame(sf::IntRect(11 * width, 0, width, height));
	casting2Animation->setLooped(false);

	addAnimation(GameObjectState::Casting2, casting2Animation);

	// heal & cast fireball
	Animation* fighting3Animation = new Animation(sf::seconds(0.1f));
	fighting3Animation->setSpriteSheet(tex);
	fighting3Animation->addFrame(sf::IntRect(20 * width, 0, width, height));
	fighting3Animation->setLooped(false);

	addAnimation(GameObjectState::Fighting3, fighting3Animation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

void RoyalguardIBoss::loadParticles() {
	loadFireLineParticles();
	loadFireParticles();
}

void RoyalguardIBoss::loadFireParticles() {
	////////////////////
	// FIRE PARTICLES //
	////////////////////

	ParticleComponentData data;
	data.particleCount = 50;
	data.emitRate = 50.f;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_FLAME;
	data.particleTexture = &dynamic_cast<LevelScreen*>(getScreen())->getParticleBGRenderTexture();

	// Generators
	auto posGen = new particles::CircleSpawner();
	posGen->radius = sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.width * m_boundingBox.width * 0.5f);
	data.spawner = posGen;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 50.f;
	sizeGen->maxStartSize = 80.f;
	sizeGen->minEndSize = 10.f;
	sizeGen->maxEndSize = 20.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();

	colGen->minStartCol = sf::Color(255, 160, 64, 0);
	colGen->maxStartCol = sf::Color(255, 160, 64, 0);
	colGen->minEndCol = sf::Color(255, 0, 0, 255);
	colGen->maxEndCol = sf::Color(255, 0, 0, 255);
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = -10.f;
	velGen->maxAngle = 10.f;
	velGen->minStartSpeed = 40.f;
	velGen->maxStartSpeed = 80.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 0.5f;
	timeGen->maxTime = 1.0f;
	data.timeGen = timeGen;

	m_firePc = new ParticleComponent(data, this);
	m_firePc->setOffset(sf::Vector2f(60.f, -20.f));
	m_firePc->setVisible(false);
	addComponent(m_firePc);
	m_pcs.push_back(m_firePc);

	// light
	addComponent(new LightComponent(LightData(
		sf::Vector2f(getBoundingBox()->width, 0.f),
		sf::Vector2f(m_boundingBox.width * 2.f, m_boundingBox.width * 2.f), 0.6f), this));
}

void RoyalguardIBoss::loadFireLineParticles() {
	////////////////////
	// LINE PARTICLES //
	////////////////////

	// line particles
	ParticleComponentData data;
	data.particleCount = 200;
	data.emitRate = 400.f;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_FLAME;
	data.particleTexture = &dynamic_cast<LevelScreen*>(getScreen())->getParticleBGRenderTexture();

	// Generators
	m_lineSpawner = new particles::LineSpawner();
	data.spawner = m_lineSpawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 40.f;
	sizeGen->maxStartSize = 60.f;
	sizeGen->minEndSize = 10.f;
	sizeGen->maxEndSize = 20.f;
	data.sizeGen = sizeGen;

	data.colorGen = ParticleTile::getFlameColorGenerator("red");

	m_lineVelGen = new particles::AngledVelocityGenerator();
	m_lineVelGen->minStartSpeed = 20.f;
	m_lineVelGen->maxStartSpeed = 50.f;
	m_lineVelGen->minAngle = -90.f;
	m_lineVelGen->maxAngle = 90.f;
	data.velGen = m_lineVelGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 0.4f;
	timeGen->maxTime = 0.6f;
	data.timeGen = timeGen;

	m_fireLinePc = new ParticleComponent(data, this);
	m_fireLinePc->setOffset(sf::Vector2f(0.5f * getBoundingBox()->width, -40.f));
	m_fireLinePc->setVisible(false);
	addComponent(m_fireLinePc);
	m_pcs.push_back(m_fireLinePc);
}

MovingBehavior* RoyalguardIBoss::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	behavior = new RoyalguardIBossMovingBehavior(this);
	behavior->setDistanceToAbyss(100.f);
	behavior->setApproachingDistance(50.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityX(200.f);
	behavior->setDropAlways(true);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* RoyalguardIBoss::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	behavior = new AggressiveBehavior(this);
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&RoyalguardIBoss::handleAttackInput, this));
	return behavior;
}

std::string RoyalguardIBoss::getSpritePath() const {
	return "res/texture/bosses/spritesheet_boss_royalguard_i.png";
}

std::string RoyalguardIBoss::getWeaponTexturePath() const {
	return "res/texture/bosses/pike_i.png";
}

std::string RoyalguardIBoss::getDeathSoundPath() const {
	return "res/sound/mob/morgiana_death.ogg";
}

/////////////////////
// ROYALGUARD FIRE //
/////////////////////

const sf::Time RoyalguardFire::GRACE_TIME = sf::seconds(2.f);
const int RoyalguardFire::FIRE_DAMAGE = 40;

const sf::Vector2f RoyalguardFire::FIRE_POS_TOP = sf::Vector2f(650.f, 160.f);
const sf::Vector2f RoyalguardFire::FIRE_POS_BOT = sf::Vector2f(650.f, 640.f);
const sf::Vector2f RoyalguardFire::FIRE_EXTENTS = sf::Vector2f(1200.f, 20.f);

RoyalguardFire::RoyalguardFire(bool isTop, LevelMovableGameObject* mainChar) {
	m_isAlwaysUpdate = true;
	m_mainChar = mainChar;
	m_screen = m_mainChar->getScreen();
	m_isTop = isTop;
	m_ttl = RoyalguardIBoss::FIRE_TIME;

	m_boundingBox.height = FIRE_EXTENTS.y;
	m_boundingBox.left = (m_isTop ? FIRE_POS_TOP : FIRE_POS_BOT).x;
	m_boundingBox.top = (m_isTop ? FIRE_POS_TOP : FIRE_POS_BOT).y - FIRE_EXTENTS.y * 0.5f;
	m_graceTime = GRACE_TIME;

	loadParticles();
}

void RoyalguardFire::update(const sf::Time& frameTime) {
	GameObject::update(frameTime);
	updateTime(m_graceTime, frameTime);
	updateTime(m_ttl, frameTime);
	updateTime(m_timeSinceHurt, frameTime);
	if (m_ttl == sf::Time::Zero) {
		setDisposed();
		return;
	}
	if (m_graceTime > sf::Time::Zero) {
		m_boundingBox.width = (1.f - m_graceTime.asSeconds() / GRACE_TIME.asSeconds()) * FIRE_EXTENTS.x;
		m_boundingBox.left = (m_isTop ? FIRE_POS_TOP : FIRE_POS_BOT).x - m_boundingBox.width * 0.5f;
		m_posGen->size.x = m_boundingBox.width;
		m_lightObject->setSize(sf::Vector2f(m_boundingBox.width * 1.5f, m_lightObject->getSize().y));
	}
	if (m_timeSinceHurt == sf::Time::Zero && fastIntersect(*m_mainChar->getBoundingBox(), m_boundingBox)) {
		m_mainChar->addDamage(FIRE_DAMAGE, DamageType::Fire, false, false);
		m_timeSinceHurt = sf::seconds(1.f);
	}
}

void RoyalguardFire::loadParticles() {
	ParticleComponentData data;
	data.particleCount = 1000;
	data.emitRate = 1000.f;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_FLAME;
	data.particleTexture = &dynamic_cast<LevelScreen*>(getScreen())->getParticleBGRenderTexture();

	// Generators
	m_posGen = new particles::BoxSpawner();
	m_posGen->center = m_isTop ? FIRE_POS_TOP : FIRE_POS_BOT;
	m_posGen->size = sf::Vector2f(0.f, FIRE_EXTENTS.y);
	data.spawner = m_posGen;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 60.f;
	sizeGen->maxStartSize = 100.f;
	sizeGen->minEndSize = 20.f;
	sizeGen->maxEndSize = 40.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();

	colGen->minStartCol = sf::Color(255, 160, 64, 0);
	colGen->maxStartCol = sf::Color(255, 160, 64, 0);
	colGen->minEndCol = sf::Color(255, 0, 0, 255);
	colGen->maxEndCol = sf::Color(255, 0, 0, 255);
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = m_isTop ? 180.f : 0.f;
	velGen->maxAngle = velGen->minAngle;
	velGen->minStartSpeed = 30.f;
	velGen->maxStartSpeed = 60.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 0.5f;
	timeGen->maxTime = 1.0f;
	data.timeGen = timeGen;

	auto pc = new ParticleComponent(data, this);
	pc->setOffset(sf::Vector2f(0.f, 0.f));
	pc->getParticleSystem()->addGenerator<particles::DirectionDefinedRotationGenerator>();
	addComponent(pc);

	// light
	m_lightObject = new LightObject(LightData(
		m_isTop ? FIRE_POS_TOP : FIRE_POS_BOT,
		sf::Vector2f(20.f, m_boundingBox.height * 4.f), 1.0f));
	m_screen->addObject(m_lightObject);
}
