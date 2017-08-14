#include "Spells/ExplosionSpell.h"
#include "GlobalResource.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "World/CustomParticleUpdaters.h"

ExplosionSpell::ExplosionSpell() : Spell() {
}

void ExplosionSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	loadComponents();

	m_particleTime = sf::seconds(5.f);
}

void ExplosionSpell::update(const sf::Time& frameTime) {
	Spell::update(frameTime);

	if (m_particleTime == sf::Time::Zero) return;

	updateTime(m_particleTime, frameTime);

	if (m_particleTime == sf::Time::Zero) {
		m_pc->setEmitRate(0.f);
	}
}

bool ExplosionSpell::getConfiguredRotateSprite() const {
	return false;
}

void ExplosionSpell::loadComponents() {
	// light
	LightData lightData(sf::Vector2f(m_boundingBox.width / 2.f, m_boundingBox.height / 2.f), 200.f, 0.8f);
	addComponent(new LightComponent(lightData, this));

	// particles
	ParticleComponentData data;
	data.texturePath = GlobalResource::TEX_PARTICLE_BEAM;
	data.isAdditiveBlendMode = true;
	data.emitRate = 200.f;
	data.particleCount = 1000;

	auto posGen = new particles::DiskSpawner();
	posGen->radius = 10.f;
	data.spawner = posGen;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 200.f;
	sizeGen->maxStartSize = 500.f;
	sizeGen->minEndSize = 200.f;
	sizeGen->maxEndSize = 500.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = sf::Color(255, 160, 64, 0);
	colGen->maxStartCol = sf::Color(255, 160, 64, 0);
	colGen->minEndCol = sf::Color(255, 0, 0, 255);
	colGen->maxEndCol = sf::Color(255, 0, 0, 255);
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartSpeed = 200.f;
	velGen->maxStartSpeed = 500.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 5.f;
	timeGen->maxTime = 5.f;
	data.timeGen = timeGen;

	m_pc = new  ParticleComponent(data, this);
	m_pc->setOffset(sf::Vector2f(getBoundingBox()->width * 0.5f, getBoundingBox()->height * 0.5f));
	m_pc->getParticleSystem()->addGenerator<particles::DirectionDefinedRotationGenerator>();
	
	m_pc->getParticleSystem()->addUpdater(new particles::CollidingUpdater(m_level));
	addComponent(m_pc);
}
