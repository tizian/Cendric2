#include "Level/Enemies/ZeffBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Boss_Zeff, ZeffBoss)

void ZeffBoss::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = 50;
	loot.insert({ "we_zeffssword", 1 });
	loot.insert({ "fo_ham", 2 });
}

void ZeffBoss::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	// nothing
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
	chopSpell.spellOffset = sf::Vector2f(-60.f, 0.f);
	chopSpell.fightingTime = sf::milliseconds(400);
	
	m_spellManager->addSpell(chopSpell);

	SpellData projectile = SpellData::getSpellData(SpellID::ReturningProjectile);
	projectile.damage = 10;
	projectile.duration = sf::seconds(2.f);
	projectile.damagePerSecond = 2;
	projectile.cooldown = sf::milliseconds(5000);
	projectile.isBlocking = true;
	projectile.fightingTime = sf::seconds(10000);
	projectile.fightAnimation = GameObjectState::Fighting2;
	projectile.castingTime = sf::milliseconds(800);
	projectile.castingAnimation = GameObjectState::Casting;
	projectile.range = 400;
	projectile.speed = 400;
	
	m_spellManager->addSpell(projectile);

	m_spellManager->setCurrentSpell(0); // chop
}

void ZeffBoss::handleAttackInput() {
	if (m_enemyAttackingBehavior->distToTarget() < 150.f) {
		m_spellManager->setCurrentSpell(0); // spin
	}
	else {
		m_spellManager->setCurrentSpell(1); // sword throw
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
	fighting2Animation->addFrame(sf::IntRect(0 * 120, 4 * 140, 120, 140));
	fighting2Animation->addFrame(sf::IntRect(1 * 120, 4 * 140, 120, 140));
	fighting2Animation->addFrame(sf::IntRect(2 * 120, 4 * 140, 120, 140));
	fighting2Animation->addFrame(sf::IntRect(1 * 120, 4 * 140, 120, 140));

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

void ZeffBoss::render(sf::RenderTarget& target) {
	Enemy::render(target);
	if (m_isDead) m_ps->render(target);
}

void ZeffBoss::setDead() {
	if (m_isDead) return;
	Enemy::setDead();
	g_resourceManager->playSound(m_sound, ResourceID::Sound_cendric_death, true);
	updateParticleSystemPosition();
}

int ZeffBoss::getMentalStrength() const {
	return 2;
}

sf::Time ZeffBoss::getConfiguredWaitingTime() const {
	return sf::Time::Zero;
}

void ZeffBoss::updateParticleSystemPosition() {
	m_posGenerator->center.x = getPosition().x + getBoundingBox()->width / 2.f;
	m_posGenerator->center.y = getPosition().y + getBoundingBox()->height * (2.f / 3.f);
}

void ZeffBoss::loadParticleSystem() {
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(300, g_resourceManager->getTexture(ResourceID::Texture_Particle_star)));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 100.f;

	// Generators
	m_posGenerator = m_ps->addGenerator<particles::DiskPositionGenerator>();
	m_posGenerator->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	m_posGenerator->radius = 20.f;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 5.f;
	sizeGen->maxStartSize = 10.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 1.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(255, 255, 255, 255);
	colGen->maxStartCol = sf::Color(255, 255, 255, 255);
	colGen->minEndCol = sf::Color(255, 255, 255, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = -45.f;
	velGen->maxAngle = 45.f;
	velGen->minStartVel = 50.f;
	velGen->maxStartVel = 70.f;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 2.f;
	timeGen->maxTime = 3.f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}


