#include "Level/Enemies/BookEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveFlyingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyFlyingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Book, BookEnemy)

void BookEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = 5;
}

void BookEnemy::insertRespawnLoot(std::map<std::string, int>& loot, int& gold) const {
	gold = rand() % 2 + 1;
}

BookEnemy::BookEnemy(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
}

void BookEnemy::update(const sf::Time& frameTime) {
	Enemy::update(frameTime);

	m_particleSpawner->center.x = getPosition().x + getBoundingBox()->width / 2;
	m_particleSpawner->center.y = getPosition().y + getBoundingBox()->height / 2;
	m_ps->update(frameTime);
}

void BookEnemy::render(sf::RenderTarget& target) {
	m_ps->render(target);
	Enemy::render(target);
}

void BookEnemy::onHit(Spell* spell) {
	Enemy::onHit(spell);

	if (spell->getSpellID() == SpellID::Chop) {
		m_ps->emitParticles(5);
		if (m_isDead) {
			spell->setDisposed();
		}
	}
}

void BookEnemy::loadAttributes() {
	m_attributes.setHealth(30);
	m_attributes.resistancePhysical = 5;
	m_attributes.resistanceIce = 10;
	m_attributes.resistanceShadow = 10;
	m_attributes.resistanceLight = 10;
	m_attributes.resistanceFire = -15;
	m_attributes.calculateAttributes();
}

void BookEnemy::loadSpells() {
	SpellData pageSpell = SpellData::getSpellData(SpellID::RotatingProjectile);
	pageSpell.isDynamicTileEffect = false;
	pageSpell.damage = 3;
	pageSpell.fightingTime = sf::seconds(0.f);
	pageSpell.castingTime = sf::seconds(0.f);
	pageSpell.castingAnimation = GameObjectState::VOID;
	pageSpell.fightAnimation = GameObjectState::VOID;
	pageSpell.speed = 400.f;
	pageSpell.cooldown = sf::seconds(1.f);

	m_spellManager->addSpell(pageSpell);
	m_spellManager->setCurrentSpell(0);
}

MovingBehavior* BookEnemy::createMovingBehavior(bool asAlly) {
	FlyingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyFlyingBehavior(this);
	}
	else {
		behavior = new AggressiveFlyingBehavior(this);
	}
	behavior->setApproachingDistance(10.f);
	behavior->setMaxVelocityYDown(200.f);
	behavior->setMaxVelocityYUp(100.f);
	behavior->setMaxVelocityX(100.f);
	return behavior;
}

AttackingBehavior* BookEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(500.f);
	behavior->setAttackInput(std::bind(&BookEnemy::handleAttackInput, this));
	return behavior;
}

void BookEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < 600.f) {
		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
		m_chasingTime = sf::Time::Zero;
		if (isAlly()) {
			m_waitingTime = sf::seconds(1);
		}
		else {
			m_waitingTime = sf::seconds(static_cast<float>(rand() % 8 + 3));
		}
	}
}

sf::Time BookEnemy::getConfiguredWaitingTime() const {
	return sf::seconds(static_cast<float>(rand() % 2 + 1));
}

sf::Time BookEnemy::getConfiguredChasingTime() const {
	return sf::seconds(static_cast<float>(rand() % 4 + 2));
}

void BookEnemy::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 70.f, 50.f));
	setSpriteOffset(sf::Vector2f(-20.f, -7.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	int width = 100;
	int height = 64;

	Animation* flyingAnimation = new Animation();
	flyingAnimation->setSpriteSheet(tex);
	flyingAnimation->addFrame(sf::IntRect(0 * width, skinNr * height, width, height));
	flyingAnimation->addFrame(sf::IntRect(1 * width, skinNr * height, width, height));
	flyingAnimation->addFrame(sf::IntRect(2 * width, skinNr * height, width, height));
	flyingAnimation->addFrame(sf::IntRect(3 * width, skinNr * height, width, height));
	flyingAnimation->addFrame(sf::IntRect(2 * width, skinNr * height, width, height));
	flyingAnimation->addFrame(sf::IntRect(1 * width, skinNr * height, width, height));

	addAnimation(GameObjectState::Flying, flyingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0 * width, skinNr * height, width, height));
	idleAnimation->addFrame(sf::IntRect(1 * width, skinNr * height, width, height));
	idleAnimation->addFrame(sf::IntRect(2 * width, skinNr * height, width, height));
	idleAnimation->addFrame(sf::IntRect(3 * width, skinNr * height, width, height));
	idleAnimation->addFrame(sf::IntRect(2 * width, skinNr * height, width, height));
	idleAnimation->addFrame(sf::IntRect(1 * width, skinNr * height, width, height));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(3 * width, skinNr * height, width, height));

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	loadParticleSystem();
}

void BookEnemy::setDead() {
	m_boundingBox.width = 70.f;
	m_boundingBox.height = 25.f;
	setSpriteOffset(sf::Vector2f(-20.f, -12.f));

	m_ps->emitParticles(10);
	Enemy::setDead();
}

void BookEnemy::loadParticleSystem() {
	m_ps = new particles::SpriteSheetParticleSystem(80, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_CONFETTI));
	m_ps->additiveBlendMode = false;
	m_ps->emitRate = 0.f;

	// Generators
	auto posGen = m_ps->addSpawner<particles::DiskSpawner>();
	posGen->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	posGen->radius = 30.f;
	m_particleSpawner = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 6.f;
	sizeGen->maxStartSize = 18.;
	sizeGen->minEndSize = 4.f;
	sizeGen->maxEndSize = 10.f;

	auto colGen = m_ps->addGenerator<particles::ConstantColorGenerator>();
	colGen->color = sf::Color(255, 255, 255, 255);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = -60.f;
	velGen->maxAngle = 60.f;
	velGen->minStartSpeed = 70.f;
	velGen->maxStartSpeed = 90.f;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 0.6f;
	timeGen->maxTime = 1.5f;

	auto texCoordGen = m_ps->addGenerator<particles::TexCoordsRandomGenerator>();
	texCoordGen->texCoords.push_back(sf::IntRect(0, 0, 8, 8));
	texCoordGen->texCoords.push_back(sf::IntRect(8, 0, 8, 8));
	texCoordGen->texCoords.push_back(sf::IntRect(16, 0, 8, 8));
	texCoordGen->texCoords.push_back(sf::IntRect(24, 0, 8, 8));

	auto rotGen = m_ps->addGenerator<particles::RotationGenerator>();
	rotGen->minStartAngle = -90.f;
	rotGen->maxStartAngle = 90.f;
	rotGen->minEndAngle = -20.f;
	rotGen->maxEndAngle = 20.f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	auto euler = m_ps->addUpdater<particles::EulerUpdater>();
	euler->globalAcceleration = sf::Vector2f(0.f, 200.f);
	m_ps->addUpdater<particles::SizeUpdater>();
	m_ps->addUpdater<particles::RotationUpdater>();
}

std::string BookEnemy::getSpritePath() const {
	return "res/assets/enemies/spritesheet_enemy_book.png";
}