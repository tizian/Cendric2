#include "Level/Enemies/RoyalguardTBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/RoyalguardTBossMovingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "Screens/LevelScreen.h"
#include "GameObjectComponents/LightComponent.h"
#include "World/CustomParticleUpdaters.h"
#include "Registrar.h"
#include "GlobalResource.h"

REGISTER_ENEMY(EnemyID::Boss_Royalguard_T, RoyalguardTBoss)

RoyalguardTBoss::RoyalguardTBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen),
	RoyalguardBoss(level, screen) {
	loadBoss();
}

void RoyalguardTBoss::loadAttributes() {
	m_attributes.setHealth(500);
	m_attributes.resistanceFire = 0;
	m_attributes.resistancePhysical = 100;
	m_attributes.resistanceIce = 10000;
	m_attributes.critical = 0;
	m_attributes.calculateAttributes();
}

void RoyalguardTBoss::loadSpells() {
	// swirl
	SpellData swirl = SpellData::getSpellData(SpellID::Chop);
	swirl.damage = 30;
	swirl.cooldown = sf::seconds(0.9f);
	swirl.fightingTime = sf::seconds(1.1f);
	swirl.activeDuration = sf::seconds(1.f);
	swirl.boundingBox = sf::FloatRect(0.f, 0.f, 120.f, 120.f);
	swirl.spellOffset = sf::Vector2f(-45.f, -30.f);
	swirl.fightAnimation = GameObjectState::Fighting;
	swirl.isBlocking = true;

	m_spellManager->addSpell(swirl);

	// charge
	SpellData charge = SpellData::getSpellData(SpellID::Chop);
	charge.damage = 40;
	charge.cooldown = sf::seconds(1.f);
	charge.activeDuration = sf::seconds(3.f);
	charge.boundingBox = sf::FloatRect(0.f, 0.f, 40.f, 40.f);
	charge.spellOffset = sf::Vector2f(20.f, 10.f);

	m_spellManager->addSpell(charge);

	m_spellManager->setCurrentSpell(0);
}

void RoyalguardTBoss::handleAttackInput() {
	// nop, handled over other functions
}

void RoyalguardTBoss::updateBossState(const sf::Time& frameTime) {
	if (m_isDead) return;
	updateTime(m_stateTime, frameTime);
	if (m_stateTime > sf::Time::Zero) return;
	switch (m_bossState)
	{
	default:
	case RoyalguardBossState::Waiting:
		if (dist(getCenter(), m_mainChar->getCenter()) > 100.f) {
			m_bossState = RoyalguardBossState::ChargingStart;
			m_weaponRotateType = WeaponRotateType::ToMainChar;
		}
		else {
			m_bossState = RoyalguardBossState::Swirl;
			m_spellManager->setCurrentSpell(0);
			m_spellManager->executeCurrentSpell(m_mainChar);
			m_weaponRotateType = WeaponRotateType::Turn;
		}
		m_stateTime = sf::seconds(1.f);
		m_acceleration = sf::Vector2f(0.f, 0.f);
		m_velocity = sf::Vector2f(0.f, 0.f);
		m_weaponOffset = sf::Vector2f(27.f, 33.f);
		m_isWeaponVisible = true;
		m_icePc->reset();
		m_icePc->setVisible(true);
		break;
	case RoyalguardBossState::ChargingStart:
		m_bossState = RoyalguardBossState::Charging;
		m_weaponRotateType = WeaponRotateType::Fixed;
		m_weaponOffset = sf::Vector2f(29.f, 25.f);
		getMovingBehavior()->setMaxVelocityX(1000.f);
		dynamic_cast<RoyalguardTBossMovingBehavior*>(getMovingBehavior())->setChargingDirection(normalized(m_mainChar->getCenter() - getCenter()));
		m_stateTime = sf::seconds(10.f);
		m_spellManager->setCurrentSpell(1);
		m_spellManager->executeCurrentSpell(m_mainChar);
		break;
	case RoyalguardBossState::Charging:
		m_isWeaponVisible = false;
		m_bossState = RoyalguardBossState::Waiting;
		getMovingBehavior()->setMaxVelocityX(200.f);
		m_stateTime = sf::seconds(2.f);
		m_icePc->setVisible(false);
		break;
	case RoyalguardBossState::Swirl:
		if (dist(getCenter(), m_mainChar->getCenter()) < 100.f) {
			setReady();
			m_spellManager->executeCurrentSpell(m_mainChar);
			m_stateTime = sf::seconds(1.f);
			break;
		}
		m_isWeaponVisible = false;
		m_icePc->setVisible(false);
		m_bossState = RoyalguardBossState::Waiting;
		break;
	case RoyalguardBossState::Healing:
		m_isWeaponVisible = false;
		m_bossState = RoyalguardBossState::Waiting;
		m_stateTime = sf::seconds(1.f);
		m_healingPc->setVisible(false);
		m_other->revive();
		break;
	}
}

void RoyalguardTBoss::loadAnimation(int skinNr) {
	int width = 120;
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

	// swirl
	Animation* fighting1Animation = new Animation();
	fighting1Animation->setSpriteSheet(tex);
	fighting1Animation->addFrame(sf::IntRect(21 * width, 0, width, height));
	fighting1Animation->setLooped(false);

	addAnimation(GameObjectState::Fighting, fighting1Animation);

	// before charge
	Animation* casting2Animation = new Animation(sf::seconds(0.1f));
	casting2Animation->setSpriteSheet(tex);
	casting2Animation->addFrame(sf::IntRect(21 * width, 0, width, height));
	casting2Animation->setLooped(false);

	addAnimation(GameObjectState::Casting2, casting2Animation);

	// charge
	Animation* fighting2Animation = new Animation();
	fighting2Animation->setSpriteSheet(tex);
	fighting2Animation->addFrame(sf::IntRect(22 * width, 0, width, height));
	fighting2Animation->setLooped(false);

	addAnimation(GameObjectState::Fighting2, fighting2Animation);

	// heal
	Animation* fighting3Animation = new Animation(sf::seconds(0.1f));
	fighting3Animation->setSpriteSheet(tex);
	fighting3Animation->addFrame(sf::IntRect(20 * width, 0, width, height));
	fighting3Animation->setLooped(false);

	addAnimation(GameObjectState::Fighting3, fighting3Animation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

void RoyalguardTBoss::loadParticles() {
	////////////////////
	// ICE PARTICLES //
	////////////////////

	ParticleComponentData data;
	data.particleCount = 100;
	data.emitRate = 100.f;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_STAR;
	data.particleTexture = &dynamic_cast<LevelScreen*>(getScreen())->getParticleBGRenderTexture();

	// Generators
	auto posGen = new particles::EllipseSpawner();
	posGen->radius = sf::Vector2f(60.f, 60.f);
	data.spawner = posGen;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 30.f;
	sizeGen->maxStartSize = 40.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 0.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();

	colGen->minStartCol = sf::Color(100, 100, 200, 255);
	colGen->maxStartCol = sf::Color(200, 255, 255, 255);
	colGen->minEndCol = sf::Color(100, 255, 255, 255);
	colGen->maxEndCol = sf::Color(255, 255, 255, 255);
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = -10.f;
	velGen->maxAngle = 10.f;
	velGen->minStartSpeed = 20.f;
	velGen->maxStartSpeed = 40.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 0.5f;
	timeGen->maxTime = 1.0f;
	data.timeGen = timeGen;

	m_icePc = new ParticleComponent(data, this);
	m_icePc->setOffset(sf::Vector2f(getBoundingBox()->width * 0.6f, getBoundingBox()->height * 0.4f));
	m_icePc->setVisible(false);
	addComponent(m_icePc);
	m_pcs.push_back(m_icePc);

	// light
	addComponent(new LightComponent(LightData(
		sf::Vector2f(getBoundingBox()->width, 0.f),
		sf::Vector2f(m_boundingBox.width * 2.f, m_boundingBox.width * 2.f), 0.6f), this));
}

MovingBehavior* RoyalguardTBoss::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	behavior = new RoyalguardTBossMovingBehavior(this);
	behavior->setDistanceToAbyss(100.f);
	behavior->setApproachingDistance(50.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityX(0.f);
	behavior->setDropAlways(true);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* RoyalguardTBoss::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	behavior = new AggressiveBehavior(this);
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&RoyalguardTBoss::handleAttackInput, this));
	return behavior;
}

std::string RoyalguardTBoss::getSpritePath() const {
	return "res/texture/bosses/spritesheet_boss_royalguard_t.png";
}

std::string RoyalguardTBoss::getWeaponTexturePath() const {
	return "res/texture/bosses/pike_t.png";
}

std::string RoyalguardTBoss::getDeathSoundPath() const {
	return "res/sound/mob/jeremy_death.ogg";
}
