#include "Level/Enemies/JanusBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "GameObjectComponents/InteractComponent.h"
#include "GameObjectComponents/LightComponent.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::Boss_Janus, JanusBoss)

const std::string JanusBoss::PARTICLE_TEX_PATH = "res/assets/particles/cloud.png";

JanusBoss::JanusBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen),
	Boss(level, screen) {
}

JanusBoss::~JanusBoss() {
	delete m_cloudPs;
}

void JanusBoss::loadAttributes() {
	m_attributes.setHealth(800);
	m_attributes.resistanceIce = 200;
	m_attributes.resistanceFire = 50;
	m_attributes.critical = 20;
	m_attributes.calculateAttributes();
}

void JanusBoss::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 80;
	chopSpell.activeDuration = sf::seconds(1.5f);
	chopSpell.cooldown = sf::seconds(4.f);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 80, 50);
	chopSpell.spellOffset = sf::Vector2f(-40.f, 0.f);
	chopSpell.fightingTime = sf::seconds(0);

	m_spellManager->addSpell(chopSpell);

	m_spellManager->setCurrentSpell(0); // chop

	m_spellManager->setGlobalCooldown(sf::seconds(2.f));
}

void JanusBoss::update(const sf::Time& frameTime) {
	m_cloudPs->update(frameTime);
	Boss::update(frameTime);
}

void JanusBoss::render(sf::RenderTarget& target) {
	m_cloudPs->render(target);
	Boss::render(target);
	m_mask.render(target);
}

void JanusBoss::handleAttackInput() {
	//m_spellManager->executeCurrentSpell(m_mainChar->getCenter());
}

void JanusBoss::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 60.f, 150.f));
	setSpriteOffset(sf::Vector2f(-70.f, -65.f));
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());
	int width = 200;
	int height = 200;

	Animation* walkingAnimation = new Animation();
	walkingAnimation->setSpriteSheet(tex);
	for (int i = 1; i < 6; ++i) {
		walkingAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}
	for (int i = 4; i > 1; --i) {
		walkingAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 6; ++i) {
		idleAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}
	for (int i = 4; i > 0; --i) {
		idleAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}
	for (int i = 6; i < 11; ++i) {
		idleAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}
	for (int i = 10; i > 6; --i) {
		idleAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(0, 0, width, height));

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	loadDeathParticleSystem();
	loadCloudParticles();

	LightData data(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.2f), sf::Vector2f(150.f, 200.f), 0.9f);
	addComponent(new LightComponent(data, this));
}

MovingBehavior* JanusBoss::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;

	behavior = new AggressiveWalkingBehavior(this);
	behavior->setApproachingDistance(100.f);
	behavior->setMaxVelocityYDown(200.f);
	behavior->setMaxVelocityYUp(200.f);
	behavior->setMaxVelocityX(70.f);
	return behavior;
}

AttackingBehavior* JanusBoss::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	behavior = new AggressiveBehavior(this);
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&JanusBoss::handleAttackInput, this));
	return behavior;
}

void JanusBoss::setPosition(const sf::Vector2f& pos) {
	Boss::setPosition(pos);
	m_mask.setPosition(pos);
	if (m_cloudSpawner) m_cloudSpawner->center = pos + sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height - 30.f);
}

sf::Time JanusBoss::getConfiguredWaitingTime() const {
	return sf::seconds(0);
}

float JanusBoss::getConfiguredDistanceToHPBar() const {
	return 60.f;
}

std::string JanusBoss::getSpritePath() const {
	return "res/assets/bosses/spritesheet_boss_janus.png";
}

void JanusBoss::loadCloudParticles() {
	g_resourceManager->loadTexture(PARTICLE_TEX_PATH, ResourceType::Level);

	sf::Texture* tex = g_resourceManager->getTexture(PARTICLE_TEX_PATH);
	if (tex == nullptr) return;

	tex->setSmooth(true);
	m_cloudPs = new particles::TextureParticleSystem(100, g_resourceManager->getTexture(PARTICLE_TEX_PATH));
	m_cloudPs->emitRate = 40.f;

	// Generators
	auto posGen = m_cloudPs->addSpawner<particles::DiskSpawner>();
	posGen->radius = 30.f;
	m_cloudSpawner = posGen;

	auto sizeGen = m_cloudPs->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 5.f;
	sizeGen->maxStartSize = 10.f;
	sizeGen->minEndSize = 30.f;
	sizeGen->maxEndSize = 40.f;

	auto colGen = m_cloudPs->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(200, 200, 200, 200);
	colGen->maxStartCol = sf::Color(210, 200, 210, 200);
	colGen->minEndCol = sf::Color(100, 100, 100, 0);
	colGen->maxEndCol = sf::Color(110, 100, 110, 0);

	auto velGen = m_cloudPs->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartSpeed = 0.f;
	velGen->maxStartSpeed = 2.f;

	auto timeGen = m_cloudPs->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 2.f;
	timeGen->maxTime = 3.f;

	// Updaters
	m_cloudPs->addUpdater<particles::TimeUpdater>();
	m_cloudPs->addUpdater<particles::ColorUpdater>();
	m_cloudPs->addUpdater<particles::EulerUpdater>();
	m_cloudPs->addUpdater<particles::SizeUpdater>();
}

//////////////////////////////////////////////////////////////
//// JANUS MASK //////////////////////////////////////////////
//////////////////////////////////////////////////////////////

JanusBossMask::JanusBossMask() {
	load();
}

void JanusBossMask::setPosition(const sf::Vector2f& pos) {
	m_sprite.setPosition(pos + sf::Vector2f(10.f, -5.f));
}

void JanusBossMask::setEvil(bool isEvil) {
	m_sprite.setTextureRect(sf::IntRect(isEvil ? 0 : 41, 0, 41, 40));
}

void JanusBossMask::render(sf::RenderTarget& target) {
	target.draw(m_sprite);
}

void JanusBossMask::load() {
	g_resourceManager->loadTexture(getSpritesheetPath(), ResourceType::Level);

	sf::Texture* tex = g_resourceManager->getTexture(getSpritesheetPath());
	if (tex == nullptr) return;

	m_sprite.setTexture(*tex);
	setEvil(true);
}

std::string JanusBossMask::getSpritesheetPath() const {
	return "res/assets/bosses/spritesheet_janus_mask.png";
}