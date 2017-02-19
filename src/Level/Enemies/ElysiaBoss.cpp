#include "Level/Enemies/ElysiaBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/ElysiaBossMovingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "GameObjectComponents/InteractComponent.h"
#include "GameObjectComponents/LightComponent.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Boss_Elysia, ElysiaBoss)

ElysiaBoss::~ElysiaBoss() {
	delete m_ps;
}

float ElysiaBoss::getConfiguredDistanceToHPBar() const {
	return 50.f;
}

ElysiaBoss::ElysiaBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
	
	m_isAlwaysUpdate = true;
	m_bossState = ElysiaBossState::Projectile;
}

void ElysiaBoss::loadAttributes() {
	m_attributes.setHealth(800);
	m_attributes.resistanceIce = 200;
	m_attributes.resistanceFire = 50;
	m_attributes.critical = 20;
	m_attributes.calculateAttributes();
}

void ElysiaBoss::loadSpells() {
	SpellData projectile = SpellData::getSpellData(SpellID::TargetingProjectile);
	projectile.cooldown = sf::seconds(5);
	projectile.activeDuration = sf::seconds(10.f);
	projectile.damageType = DamageType::Ice;
	projectile.damage = 20;
	projectile.damagePerSecond = 5;
	projectile.speed = 400;
	projectile.count = 3;
	projectile.strength = 1;
	projectile.divergenceAngle = 0.4f;
	projectile.duration = sf::seconds(2.f);
	projectile.fightingTime = sf::seconds(0.f);
	projectile.castingTime = sf::milliseconds(12 * 100);
	projectile.spellOffset = sf::Vector2f(40.f, -40.f);

	m_spellManager->addSpell(projectile);

	projectile.cooldown = sf::seconds(8);
	projectile.activeDuration = sf::seconds(10.f);
	projectile.damageType = DamageType::Shadow;
	projectile.damage = 10;
	projectile.damagePerSecond = 4;
	projectile.speed = 400;
	projectile.count = 1;
	projectile.strength = 2;
	projectile.duration = sf::seconds(3.f);
	projectile.isStunning = true;
	projectile.skinNr = 1;
	projectile.castingTime = sf::milliseconds(6 * 100);
	projectile.castingAnimation = GameObjectState::Casting2;

	m_spellManager->addSpell(projectile);

	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 80;
	chopSpell.activeDuration = sf::seconds(1.5f);
	chopSpell.cooldown = sf::seconds(4.f);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 80, 50);
	chopSpell.spellOffset = sf::Vector2f(-40.f, 0.f);
	chopSpell.fightingTime = sf::seconds(0);

	m_spellManager->addSpell(chopSpell);

	m_spellManager->setCurrentSpell(0); // targeting projectile

	m_spellManager->setGlobalCooldown(sf::seconds(2.f));
}

void ElysiaBoss::handleAttackInput() {
	switch (m_bossState) {
	case ElysiaBossState::Projectile:
		m_spellManager->setCurrentSpell(rand() % 2); // stun or projectile
		break;
	case ElysiaBossState::Nosedive:
		m_spellManager->setCurrentSpell(2); // chop
		break;
	default:
		return;
	}
	
	m_spellManager->executeCurrentSpell(m_mainChar->getCenter());
}

void ElysiaBoss::update(const sf::Time& frameTime) {
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

void ElysiaBoss::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 80.f, 50.f));
	setSpriteOffset(sf::Vector2f(-50.f, -50.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	int width = 180;
	int height = 150;

	Animation* flyingAnimation = new Animation();
	flyingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 6; ++i) {
		flyingAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}

	addAnimation(GameObjectState::Flying, flyingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 6; ++i) {
		idleAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(0, 0, width, height));

	addAnimation(GameObjectState::Dead, deadAnimation);

	// nose dive
	Animation* fightingAnimation = new Animation();
	fightingAnimation->setSpriteSheet(tex);
	fightingAnimation->addFrame(sf::IntRect(6 * width, 0, width, height));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	// casting before thunder attack
	Animation* casting3Animation = new Animation();
	casting3Animation->setSpriteSheet(tex);
	for (int i = 0; i < 6; ++i) {
		casting3Animation->addFrame(sf::IntRect(i * width, height, width, height));
	}

	addAnimation(GameObjectState::Casting3, casting3Animation);

	// casting before projectile attack
	Animation* castingAnimation = new Animation();
	castingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 6; ++i) {
		castingAnimation->addFrame(sf::IntRect(i * width, 2 * height, width, height));
	}
	for (int i = 0; i < 6; ++i) {
		castingAnimation->addFrame(sf::IntRect(i * width, 3 * height, width, height));
	}

	addAnimation(GameObjectState::Casting, castingAnimation);

	// casting before stunning projectile attack
	Animation* casting2Animation = new Animation();
	casting2Animation->setSpriteSheet(tex);
	for (int i = 0; i < 6; ++i) {
		casting2Animation->addFrame(sf::IntRect(i * width, 4 * height, width, height));
	}

	addAnimation(GameObjectState::Casting2, casting2Animation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	loadParticleSystem();

	LightData data(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f), 150.f, 0.5f);
	addComponent(new LightComponent(data, this));
}

MovingBehavior* ElysiaBoss::createMovingBehavior(bool asAlly) {
	FlyingBehavior* behavior;

	behavior = new ElysiaBossMovingBehavior(this);
	behavior->setApproachingDistance(100.f);
	behavior->setMaxVelocityYDown(200.f);
	behavior->setMaxVelocityYUp(200.f);
	behavior->setMaxVelocityX(200.f);
	return behavior;
}

AttackingBehavior* ElysiaBoss::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	behavior = new AggressiveBehavior(this);
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&ElysiaBoss::handleAttackInput, this));
	return behavior;
}

void ElysiaBoss::render(sf::RenderTarget& target) {
	Enemy::render(target);
	if (m_isDead) m_ps->render(target);
}

void ElysiaBoss::setDead() {
	if (m_isDead) return;
	Enemy::setDead();
	updateParticleSystemPosition();
}

int ElysiaBoss::getMentalStrength() const {
	return 4;
}

sf::Time ElysiaBoss::getConfiguredWaitingTime() const {
	if (m_bossState == ElysiaBossState::Projectile) {
		return sf::seconds(1);
	}

	return sf::seconds(0);
}

void ElysiaBoss::updateParticleSystemPosition() {
	m_particleSpawner->center.x = getPosition().x + getBoundingBox()->width / 2.f;
	m_particleSpawner->center.y = getPosition().y + getBoundingBox()->height * (2.f / 3.f);
}

void ElysiaBoss::loadParticleSystem() {
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

std::string ElysiaBoss::getSpritePath() const {
	return "res/assets/bosses/spritesheet_boss_elysia.png";
}

std::string ElysiaBoss::getDeathSoundPath() const {
	return "res/sound/mob/cendric_death.ogg";
}
