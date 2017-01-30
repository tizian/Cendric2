#include "Level/Enemies/ElysiaBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/ElysiaBossMovingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "GameObjectComponents/InteractComponent.h"
#include "GameObjectComponents/LightComponent.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Boss_Elysia, ElysiaBoss)

void ElysiaBoss::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = 50;
	//loot.insert({ "mi_firstguardianheart", 1 });
	loot.insert({ "fo_rawchicken", 2 });
	loot.insert({ "fo_egg", 1 });
}

ElysiaBoss::~ElysiaBoss() {
	delete m_ps;
}

void ElysiaBoss::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	// nothing
}

float ElysiaBoss::getConfiguredDistanceToHPBar() const {
	return 60.f;
}

ElysiaBoss::ElysiaBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
	
	m_isInvincible = true;
	m_isAlwaysUpdate = true;
}

void ElysiaBoss::loadAttributes() {
	m_attributes.setHealth(150);
	m_attributes.resistanceIce = -20;
	m_attributes.resistancePhysical = 50;
	m_attributes.critical = 0;
	m_attributes.calculateAttributes();
}

void ElysiaBoss::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 50;
	chopSpell.activeDuration = sf::seconds(0.2f);
	chopSpell.cooldown = sf::seconds(10.f);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 120, 140);
	chopSpell.spellOffset = sf::Vector2f(-20.f, -50.f);
	chopSpell.fightingTime = sf::seconds(0.2f);
	chopSpell.fightAnimation = GameObjectState::Fighting;

	m_spellManager->addSpell(chopSpell);

	m_spellManager->setCurrentSpell(0); // chop
}

void ElysiaBoss::handleAttackInput() {
	// TODO
	
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
	Animation* casting2Animation = new Animation();
	casting2Animation->setSpriteSheet(tex);
	for (int i = 0; i < 6; ++i) {
		casting2Animation->addFrame(sf::IntRect(i * width, height, width, height));
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
	behavior->setApproachingDistance(10000.f);
	behavior->setMaxVelocityYDown(500.f);
	behavior->setMaxVelocityYUp(500.f);
	behavior->setMaxVelocityX(500.f);
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
	return sf::Time::Zero;
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
