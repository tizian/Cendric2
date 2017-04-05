#include "Spells/LightSpell.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "GlobalResource.h"

void LightSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	loadComponents();
}

void LightSpell::execOnHit(LevelMovableGameObject* target) {
	// nop
}

void LightSpell::update(const sf::Time& frameTime) {
	sf::Vector2f nextPosition;
	calculatePositionAccordingToMob(nextPosition, m_mob);
	setPosition(nextPosition);

	Spell::update(frameTime);

	updateTime(m_data.activeDuration, frameTime);
	if (m_data.activeDuration == sf::Time::Zero) {
		setDisposed();
	}
}

void LightSpell::loadComponents() {
	// light
	addComponent(new LightComponent(LightData(sf::Vector2f(), m_data.range), this));

	// particles
	ParticleComponentData data;
	data.particleCount = 50;
	data.isAdditiveBlendMode = true;
	data.emitRate = 25.f;
	data.texturePath = GlobalResource::TEX_PARTICLE_BLOB;
	
	// Generators
	auto spawner = new particles::BoxSpawner();
	spawner->size = sf::Vector2f(getBoundingBox()->width, 0.f);
	data.spawner = spawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 30.f;
	sizeGen->maxStartSize = 40.f;
	sizeGen->minEndSize = 20.f;
	sizeGen->maxEndSize = 30.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = sf::Color(255, 255, 204, 100);
	colGen->maxStartCol = sf::Color(255, 255, 255, 150);
	colGen->minEndCol = sf::Color(255, 255, 0, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartSpeed = 5.f;
	velGen->maxStartSpeed = 10.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 0.3f;
	timeGen->maxTime = 0.3f;
	data.timeGen = timeGen;

	auto pc = new ParticleComponent(data, this);
	pc->setOffset(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f));
	addComponent(pc);
}

