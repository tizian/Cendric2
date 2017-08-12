#include "Level/Enemies/YashaBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/YashaBossMovingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "GameObjectComponents/InteractComponent.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "GameObjectComponents/LightComponent.h"
#include "Screens/LevelScreen.h"
#include "Registrar.h"
#include "GlobalResource.h"

REGISTER_ENEMY(EnemyID::Boss_Yasha, YashaBoss)

float YashaBoss::getConfiguredDistanceToHPBar() const {
	return 80.f;
}

YashaBoss::YashaBoss(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen),
	Boss(level, screen) {

	m_isInvincible = true;
}

void YashaBoss::loadAttributes() {
	m_attributes.setHealth(2000);
	m_attributes.resistanceIce = -20;
	m_attributes.resistancePhysical = 50;
	m_attributes.critical = 0;
	m_attributes.calculateAttributes();
}

void YashaBoss::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 50;
	chopSpell.activeDuration = sf::seconds(1000.f);
	chopSpell.cooldown = sf::seconds(0.f);
	chopSpell.boundingBox = sf::FloatRect(0, 0, 120, 140);
	chopSpell.spellOffset = sf::Vector2f(-20.f, -50.f);
	chopSpell.fightingTime = sf::seconds(1000.f);
	chopSpell.castingTime = sf::seconds(1.f);
	chopSpell.castingAnimation = GameObjectState::Casting;
	chopSpell.fightAnimation = GameObjectState::Walking;

	m_spellManager->addSpell(chopSpell);

	m_spellManager->setCurrentSpell(0); // chop
}

void YashaBoss::handleAttackInput() {

}

void YashaBoss::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 200.f, 90.f));
	setSpriteOffset(sf::Vector2f(-50.f, -160.f));
	const int width = 300;
	const int height = 250;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* flyingAnimation = new Animation(sf::seconds(0.08f));
	flyingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 13; ++i) {
		flyingAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}

	addAnimation(GameObjectState::Flying, flyingAnimation);

	Animation* idleAnimation = new Animation(sf::seconds(0.08f));
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 13; ++i) {
		idleAnimation->addFrame(sf::IntRect(i * width, 0, width, height));
	}

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	loadDeathParticles();
	loadComponents();
}

MovingBehavior* YashaBoss::createMovingBehavior(bool asAlly) {
	FlyingBehavior* behavior;

	behavior = new YashaBossMovingBehavior(this);
	behavior->setApproachingDistance(10000.f);
	behavior->setMaxVelocityYDown(200.f);
	behavior->setMaxVelocityYUp(200.f);
	behavior->setMaxVelocityX(200.f);
	return behavior;
}

AttackingBehavior* YashaBoss::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	behavior = new AggressiveBehavior(this);
	behavior->setAggroRange(10000.f);
	behavior->setAttackInput(std::bind(&YashaBoss::handleAttackInput, this));
	return behavior;
}

void YashaBoss::setPosition(const sf::Vector2f& pos) {
	Boss::setPosition(pos);
	if (m_velGen) {
		m_velGen->goal = sf::Vector2f(getPosition().x + 0.5f * getBoundingBox()->width, getPosition().y - getBoundingBox()->height);
	}
}

sf::Time YashaBoss::getConfiguredWaitingTime() const {
	return sf::Time::Zero;
}

std::string YashaBoss::getSpritePath() const {
	return "res/assets/bosses/spritesheet_boss_yasha.png";
}

void YashaBoss::loadComponents() {
	ParticleComponentData data;
	data.particleCount = 300;
	data.emitRate = 300.f;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_FLAME;
	data.particleTexture = &dynamic_cast<LevelScreen*>(getScreen())->getParticleFGRenderTexture();

	// Generators
	auto posGen = new particles::BoxSpawner();
	posGen->size = sf::Vector2f(m_boundingBox.width, 0.f);
	data.spawner = posGen;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 30.f;
	sizeGen->maxStartSize = 50.f;
	sizeGen->minEndSize = 30.f;
	sizeGen->maxEndSize = 50.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();

	colGen->minStartCol = sf::Color(255, 160, 64);
	colGen->maxStartCol = sf::Color(255, 160, 64);
	colGen->minEndCol = sf::Color(255, 0, 0, 200);
	colGen->maxEndCol = sf::Color(255, 0, 0, 200);
	data.colorGen = colGen;

	m_velGen = new particles::AimedVelocityGenerator();
	m_velGen->goal = sf::Vector2f(getPosition().x + 0.5f * getBoundingBox()->width, getPosition().y - 10.f);
	m_velGen->minStartSpeed = 40.f;
	m_velGen->maxStartSpeed = 80.f;
	data.velGen = m_velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 0.3f;
	timeGen->maxTime = 0.8f;
	data.timeGen = timeGen;

	m_pc = new ParticleComponent(data, this);
	m_pc->setOffset(sf::Vector2f(0.5f * getBoundingBox()->width, getBoundingBox()->height));
	addComponent(m_pc);

	// light
	addComponent(new LightComponent(LightData(
		sf::Vector2f(TILE_SIZE_F * 0.5f, -TILE_SIZE_F * 0.5f),
		sf::Vector2f(m_boundingBox.width * 2.f, m_boundingBox.height * 2.f), 0.6f), this));
}

