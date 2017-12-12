#include "Level/Enemies/VeliusVictim.h"
#include "Screens/LevelScreen.h"
#include "Registrar.h"
#include "Level/MOBBehavior/MovingBehaviors/VeliusVictimMovingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "Level/DamageNumbers.h"

REGISTER_ENEMY(EnemyID::VeliusVictim, VeliusVictim)

VeliusVictim::VeliusVictim(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
	m_isAlwaysUpdate = true;
	m_isInvincible = true;
}

void VeliusVictim::update(const sf::Time& frameTime) {
	Enemy::update(frameTime);

	if (m_isDead) {
		updateTime(m_fadingTime, frameTime);
		updateTime(m_particleTime, frameTime);
		if (m_particleTime == sf::Time::Zero) {
			m_deathPc->setEmitRate(0.f);
		}
		setSpriteColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(m_fadingTime.asSeconds() / 2.f * 255.f)), sf::seconds(1000));
		if (m_fadingTime == sf::Time::Zero) {
			setDisposed();
		}
		return;
	}

	if (m_isDead) return;

	if (m_state == GameObjectState::Broken) {
		updateTime(m_timeUntilDamage, frameTime);
		if (m_timeUntilDamage == sf::Time::Zero) {
			m_timeUntilDamage = sf::seconds(1.f);
			m_damageNumbers->emitNumber(1, sf::Vector2f(getPosition().x + 0.5f * getSize().x, getPosition().y), DamageNumberType::Damage, false);
			m_attributes.currentHealthPoints -= 1;
			if (m_attributes.currentHealthPoints == 0) {
				setDead();
			}
		}
	}
}

void VeliusVictim::setDead() {
	Enemy::setDead();
	m_deathPc->setVisible(true);
}

void VeliusVictim::callToDie() {
	dynamic_cast<VeliusVictimMovingBehavior*>(m_movingBehavior)->callToDie();
}

void VeliusVictim::release() {
	dynamic_cast<VeliusVictimMovingBehavior*>(m_movingBehavior)->release();
}

void VeliusVictim::loadAttributes() {
	m_attributes.setHealth(200);
	m_attributes.calculateAttributes();
}

void VeliusVictim::handleAttackInput() {}

void VeliusVictim::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 30.f, 90.f));
	setSpriteOffset(sf::Vector2f(-45.f, -30.f));
	int width = 120;
	int height = 170;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* walkingAnimation = new Animation();
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 8; ++i) {
		walkingAnimation->addFrame(sf::IntRect(i * width, skinNr * height, width, height));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(8 * width, skinNr * height, width, height));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(tex);
	jumpingAnimation->addFrame(sf::IntRect(9 * width, skinNr * height, width, height));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* kneelingAnimation = new Animation();
	kneelingAnimation->setSpriteSheet(tex);
	kneelingAnimation->addFrame(sf::IntRect(10 * width, skinNr * height, width, height));

	addAnimation(GameObjectState::Inactive, kneelingAnimation);

	Animation* layingAnimation = new Animation();
	layingAnimation->setSpriteSheet(tex);
	layingAnimation->addFrame(sf::IntRect(11 * width, skinNr * height, width, height));

	addAnimation(GameObjectState::Broken, layingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(11 * width, skinNr * height, width, height));

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Inactive);
	playCurrentAnimation(true);

	loadDeathParticles();

	setAlly(sf::Time::Zero);
}

MovingBehavior* VeliusVictim::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior = new VeliusVictimMovingBehavior(this);

	behavior->setDistanceToAbyss(10.f);
	behavior->setApproachingDistance(10.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityYDown(800.f);
	behavior->setMaxVelocityX(200.f);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* VeliusVictim::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	behavior = new AllyBehavior(this);
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&VeliusVictim::handleAttackInput, this));
	return behavior;
}

std::string VeliusVictim::getSpritePath() const {
	return "res/texture/bosses/spritesheet_boss_veliusvictim.png";
}

std::string VeliusVictim::getDeathSoundPath() const {
	switch(m_skinNr)
	{
	case 2:
		return "res/sound/mob/morgiana_death.ogg";
	case 1:
		return "res/sound/mob/jeremy_death.ogg";
	default:
		return "res/sound/mob/roy_death.ogg";
	}
}

void VeliusVictim::loadDeathParticles() {
	ParticleComponentData data;
	data.particleCount = 300;
	data.emitRate = 100.f;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_STAR;

	// Generators
	auto spawner = new particles::DiskSpawner();
	spawner->radius = 20.f;
	data.spawner = spawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 20.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 2.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = sf::Color(255, 255, 255, 255);
	colGen->maxStartCol = sf::Color(255, 255, 255, 255);
	colGen->minEndCol = sf::Color(255, 255, 255, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = -45.f;
	velGen->maxAngle = 45.f;
	velGen->minStartSpeed = 50.f;
	velGen->maxStartSpeed = 70.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 2.f;
	timeGen->maxTime = 3.f;
	data.timeGen = timeGen;

	m_deathPc = new ParticleComponent(data, this);
	m_deathPc->setOffset(sf::Vector2f(getBoundingBox()->width * 0.5f, getBoundingBox()->height * 0.8f));
	m_deathPc->setVisible(false);
	addComponent(m_deathPc);
}
