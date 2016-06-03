#include "Level/Enemies/NekomataEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"
#include "GlobalResource.h"

REGISTER_ENEMY(EnemyID::Nekomata, NekomataEnemy)

void NekomataEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	loot.insert({ "mi_neko_fur", 1 });
	loot.insert({ "mi_neko_teeth", 1 });
	gold = rand() % 10 + 2;
}

void NekomataEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 5 + 2;
}

NekomataEnemy::NekomataEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
	load(EnemyID::Nekomata);
}

void NekomataEnemy::loadAttributes() {
	m_attributes.setHealth(300);
	m_attributes.resistanceLight = -30;
	m_attributes.calculateAttributes();

	m_immuneDamageTypes.push_back(DamageType::Shadow);
}

void NekomataEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 50;
	chopSpell.activeDuration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(1000);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 70, 50);
	chopSpell.fightingTime = sf::milliseconds(12 * 60);
	chopSpell.spellOffset = sf::Vector2f(20.f, 20.f);

	SpellData shadowFireSpell = SpellData::getSpellData(SpellID::HolyFire);
	shadowFireSpell.skinNr = 1;
	shadowFireSpell.damagePerSecond = 10;
	shadowFireSpell.duration = sf::seconds(3.f);
	shadowFireSpell.cooldown = sf::seconds(3.f);
	shadowFireSpell.damageType = DamageType::Shadow;
	shadowFireSpell.fightingTime = sf::milliseconds(6 * 60);
	shadowFireSpell.range = 150.f;

	SpellData shadowFireBallSpell = SpellData::getSpellData(SpellID::FireBall);
	shadowFireBallSpell.skinNr = 2;
	shadowFireBallSpell.damagePerSecond = 15;
	shadowFireBallSpell.damage = 20;
	shadowFireBallSpell.count = 2;
	shadowFireBallSpell.duration = sf::seconds(2.f);
	shadowFireBallSpell.cooldown = sf::seconds(2.f);
	shadowFireBallSpell.damageType = DamageType::Shadow;
	shadowFireBallSpell.fightingTime = sf::milliseconds(6 * 60);
	shadowFireBallSpell.spellOffset = sf::Vector2f(20.f, 20.f);

	m_spellManager->addSpell(chopSpell);
	m_spellManager->addSpell(shadowFireSpell);
	m_spellManager->addSpell(shadowFireBallSpell);
	m_spellManager->setCurrentSpell(0); // chop
}

MovingBehavior* NekomataEnemy::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyWalkingBehavior(this);
	}
	else {
		behavior = new AggressiveWalkingBehavior(this);
	}
	behavior->setDistanceToAbyss(60.f);
	behavior->setApproachingDistance(30.f);
	behavior->setMaxVelocityYDown(600.f);
	behavior->setMaxVelocityYUp(600.f);
	behavior->setMaxVelocityX(90.f);
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* NekomataEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(500.f);
	behavior->setAttackInput(std::bind(&NekomataEnemy::handleAttackInput, this));
	return behavior;
}

void NekomataEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < 300.f) {
		m_spellManager->setCurrentSpell(1); // shadow fire
		if (m_enemyAttackingBehavior->distToTarget() < 50.f) {
			m_spellManager->setCurrentSpell(0); // chop
		}
	}
	else {
		m_spellManager->setCurrentSpell(2); // shadow fire ball
	}
	m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
}

void NekomataEnemy::loadAnimation() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 90.f, 60.f));
	setSpriteOffset(sf::Vector2f(-20.f, -10.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* walkingAnimation = new Animation();
	walkingAnimation->setSpriteSheet(tex);

	for (int i = 0; i < 12; i++) {
		walkingAnimation->addFrame(sf::IntRect(0, i * 70, 130, 70));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setLooped(false);
	jumpingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 3; i++) {
		jumpingAnimation->addFrame(sf::IntRect(130, i * 70, 130, 70));
	}

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(260, 0, 130, 70));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* fightingAnimation = new Animation(sf::seconds(0.06f));
	fightingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 6; i++) {
		fightingAnimation->addFrame(sf::IntRect(390, i * 70, 130, 70));
	}

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(520, 0, 130, 70));

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	loadParticleSystem();
}

int NekomataEnemy::getMentalStrength() const {
	return 3;
}

void NekomataEnemy::update(const sf::Time& frameTime) {
	Enemy::update(frameTime);
	m_ps->update(frameTime);
	updateParticleSystemPosition();
}

void NekomataEnemy::render(sf::RenderTarget& target) {
	Enemy::render(target);
	m_ps->render(target);
}

void NekomataEnemy::loadParticleSystem() {
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(80, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_BLOB)));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 40.f;

	// Generators
	auto posGen = m_ps->addGenerator<particles::BoxPositionGenerator>();
	posGen->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	posGen->size = sf::Vector2f(getBoundingBox()->width / 2.f, getBoundingBox()->height / 2.f);
	m_posGenerator = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 5.f;
	sizeGen->maxStartSize = 20.f;
	sizeGen->minEndSize = 20.f;
	sizeGen->maxEndSize = 40.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(24, 21, 57, 80);
	colGen->maxStartCol = sf::Color(51, 51, 71, 100);
	colGen->minEndCol = sf::Color(166, 167, 198, 0);
	colGen->maxEndCol = sf::Color(198, 199, 210, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = -45.f;
	velGen->maxAngle = 45.f;
	velGen->minStartVel = 50.f;
	velGen->maxStartVel = 70.f;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 2.f;
	timeGen->maxTime = 2.f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}

void NekomataEnemy::updateParticleSystemPosition() {
	m_posGenerator->center.x = getPosition().x + getBoundingBox()->width / 2;
	m_posGenerator->center.y = getPosition().y + getBoundingBox()->height / 2;
}

std::string NekomataEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_nekomata.png";
}

