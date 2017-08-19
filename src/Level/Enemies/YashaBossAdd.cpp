#include "Level/Enemies/YashaBossAdd.h"
#include "Level/LevelMainCharacter.h"
#include "Level/DynamicTiles/ParticleTile.h"
#include "GameObjectComponents/InteractComponent.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "GameObjectComponents/LightComponent.h"
#include "Screens/LevelScreen.h"
#include "World/CustomParticleUpdaters.h"
#include "Registrar.h"
#include "GlobalResource.h"

REGISTER_ENEMY(EnemyID::YashaAdd, YashaBossAdd)

YashaBossAdd::YashaBossAdd(const Level* level, Screen* screen) :
	LevelMovableGameObject(level),
	Enemy(level, screen) {
}

void YashaBossAdd::update(const sf::Time& frameTime) {
	Enemy::update(frameTime);
	if (m_isDead) return;

	// update line spawner
	m_lineSpawner->point1 = getCenter();
	m_lineSpawner->point2 = m_skinNr == 0 ? m_mainChar->getCenter() : m_boss->getCenter();
	auto dir = m_lineSpawner->point2 - m_lineSpawner->point1;
	dir = dir / norm(dir);

	m_lineVelGen->minAngle = radToDeg(std::atan2(dir.y, dir.x));
	m_lineVelGen->maxAngle = m_lineVelGen->minAngle;

	updateTime(m_hotDotTick, frameTime);
	if (m_hotDotTick == sf::Time::Zero) {
		m_hotDotTick = sf::seconds(1.f);
		switch (m_skinNr) {
		case 0:
		default:
			m_mainChar->addDamage(1, DamageType::Fire, false, false);
			break;
		case 1:
			m_boss->addHeal(1, false, false);
			break;
		case 2:
			m_boss->setInvincible(true);
			break;
		}
	}
}

void YashaBossAdd::setDead() {
	Enemy::setDead();
	m_pc->setEmitRate(0.f);
	m_spellPc->setEmitRate(0.f);

	if (m_skinNr == 2) {
		m_boss->setInvincible(false);
	}
}

void YashaBossAdd::setBoss(LevelMovableGameObject* boss) {
	m_boss = boss;
}

void YashaBossAdd::loadAttributes() {
	m_attributes.setHealth(2000);
	m_attributes.resistanceIce = -20;
	m_attributes.resistancePhysical = 50;
	m_attributes.critical = 0;
	m_attributes.calculateAttributes();
}

void YashaBossAdd::handleAttackInput() {
}

void YashaBossAdd::loadAnimation(int skinNr) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 70.f, 125.f));
	setSpriteOffset(sf::Vector2f(-116.f, -125.f));
	const int width = 300;
	const int height = 250;
	
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect((4 + skinNr) * width, height, width, height));
	idleAnimation->setLooped(false);
	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(tex);
	deadAnimation->addFrame(sf::IntRect(7 * width, height, width, height));
	deadAnimation->setLooped(false);
	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(false);

	loadComponents();
}

MovingBehavior* YashaBossAdd::createMovingBehavior(bool asAlly) {
	return nullptr;
}

AttackingBehavior* YashaBossAdd::createAttackingBehavior(bool asAlly) {
	return nullptr;
}

std::string YashaBossAdd::getSpritePath() const {
	return "res/assets/bosses/spritesheet_boss_yasha.png";
}

void YashaBossAdd::loadComponents() {
	ParticleComponentData data;
	data.particleCount = 50;
	data.emitRate = 50.f;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_STAR;
	data.particleTexture = &dynamic_cast<LevelScreen*>(getScreen())->getParticleBGRenderTexture();

	// Generators
	auto posGen = new particles::EllipseSpawner();
	posGen->radius = sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.25f);
	data.spawner = posGen;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 20.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 10.f;
	data.sizeGen = sizeGen;

	switch (m_skinNr) {
	case 0:
	default:
		data.colorGen = ParticleTile::getEmberColorGenerator("red");
		break;
	case 1:
		data.colorGen = ParticleTile::getEmberColorGenerator("green");
		break;
	case 2:
		data.colorGen = ParticleTile::getEmberColorGenerator("blue");
		break;
	}

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minStartSpeed = 20.f;
	velGen->maxStartSpeed = 50.f;
	velGen->minAngle = -45.f;
	velGen->maxAngle = 45.f;
	data.velGen = velGen;

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

	loadLineParticles();
}

void YashaBossAdd::loadLineParticles() {
	// line particles
	ParticleComponentData data;
	data.particleCount = 300;
	data.emitRate = 300.f;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_STAR;
	data.particleTexture = &dynamic_cast<LevelScreen*>(getScreen())->getParticleBGRenderTexture();

	// Generators
	m_lineSpawner = new particles::LineSpawner();
	data.spawner = m_lineSpawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 20.f;
	sizeGen->minEndSize = 10.f;
	sizeGen->maxEndSize = 10.f;
	data.sizeGen = sizeGen;

	switch (m_skinNr) {
	case 0:
	default:
		data.colorGen = ParticleTile::getEmberColorGenerator("red");
		break;
	case 1:
		data.colorGen = ParticleTile::getEmberColorGenerator("green");
		break;
	case 2:
		data.colorGen = ParticleTile::getEmberColorGenerator("blue");
		break;
	}

	m_lineVelGen = new particles::AngledVelocityGenerator();
	m_lineVelGen->minStartSpeed = 20.f;
	m_lineVelGen->maxStartSpeed = 50.f;
	m_lineVelGen->minAngle = -90.f;
	m_lineVelGen->maxAngle = 90.f;
	data.velGen = m_lineVelGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 0.5f;
	timeGen->maxTime = 1.0f;
	data.timeGen = timeGen;

	m_spellPc = new ParticleComponent(data, this);
	m_spellPc->setOffset(sf::Vector2f(0.5f * getBoundingBox()->width, 0.f));
	addComponent(m_spellPc);
}

