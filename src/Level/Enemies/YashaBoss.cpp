#include "Level/Enemies/YashaBoss.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/YashaBossMovingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "GameObjectComponents/InteractComponent.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "GameObjectComponents/LightComponent.h"
#include "Screens/LevelScreen.h"
#include "World/CustomParticleUpdaters.h"
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
	SpellData explosionSpell = SpellData::getSpellData(SpellID::WindGust);
	explosionSpell.id = SpellID::Explosion;
	explosionSpell.activeDuration = sf::seconds(6.0f);
	explosionSpell.damagePerSecond = 0;
	explosionSpell.damageType = DamageType::VOID;
	explosionSpell.cooldown = sf::seconds(10.f);
	explosionSpell.boundingBox = sf::FloatRect(0, 0, 50, 50);
	explosionSpell.spellOffset = sf::Vector2f(-25.f, -120.f);
	explosionSpell.fightingTime = sf::seconds(3.f);
	explosionSpell.castingTime = sf::seconds(2.f);
	explosionSpell.castingAnimation = GameObjectState::Casting;
	explosionSpell.fightAnimation = GameObjectState::Fighting;
	explosionSpell.soundPath = "res/sound/spell/transformbeam.ogg";

	m_spellManager->addSpell(explosionSpell);

	m_spellManager->setCurrentSpell(0); // chop
}

void YashaBoss::handleAttackInput() {
	if (m_spellManager->executeCurrentSpell(m_mainChar->getCenter())) {
		//m_level->setAmbientDimming(1.f);
		//m_level->setLightDimming(0.f);
	}
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

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 4; ++i) {
		idleAnimation->addFrame(sf::IntRect(i * width, height, width, height));
	}
	idleAnimation->addFrame(sf::IntRect(2 * width, height, width, height));
	idleAnimation->addFrame(sf::IntRect(1 * width, height, width, height));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* castingAnimation = new Animation();
	castingAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 7; ++i) {
		castingAnimation->addFrame(sf::IntRect(i * width, 2 * height, width, height));
	}
	castingAnimation->setLooped(false);

	addAnimation(GameObjectState::Casting, castingAnimation);

	Animation* fightingAnimation = new Animation();
	fightingAnimation->setSpriteSheet(tex);
	fightingAnimation->addFrame(sf::IntRect(6 * width, 2 * height, width, height));
	fightingAnimation->setLooped(false);

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);

	loadDeathParticles();
	loadComponents();
}

MovingBehavior* YashaBoss::createMovingBehavior(bool asAlly) {
	FlyingBehavior* behavior;

	behavior = new YashaBossMovingBehavior(this);
	behavior->setApproachingDistance(120.f);
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
	data.particleTexture = &dynamic_cast<LevelScreen*>(getScreen())->getParticleBGRenderTexture();

	// Generators
	auto posGen = new particles::EllipseSpawner();
	posGen->radius = sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.25f);
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

	m_velGen = new particles::AimedVelocityGenerator();
	m_velGen->goal = sf::Vector2f(getPosition().x + 0.5f * getBoundingBox()->width, getPosition().y - 10.f);
	m_velGen->minStartSpeed = 40.f;
	m_velGen->maxStartSpeed = 80.f;
	data.velGen = m_velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 0.5f;
	timeGen->maxTime = 1.0f;
	data.timeGen = timeGen;

	m_pc = new ParticleComponent(data, this);
	m_pc->setOffset(sf::Vector2f(0.5f * getBoundingBox()->width, getBoundingBox()->height));
	addComponent(m_pc);

	// light
	addComponent(new LightComponent(LightData(
		sf::Vector2f(getBoundingBox()->width * 0.5f, getBoundingBox()->height),
		sf::Vector2f(m_boundingBox.width * 2.f, m_boundingBox.height * 2.f), 0.6f), this));
}

