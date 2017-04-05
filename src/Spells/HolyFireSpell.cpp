#include "Spells/HolyFireSpell.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "GlobalResource.h"

void HolyFireSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	loadComponents();
}

void HolyFireSpell::execOnHit(LevelMovableGameObject* target) {
	m_hasDamaged = true;
}

void HolyFireSpell::update(const sf::Time& frameTime) {
	Spell::update(frameTime);
	if (m_emitTime > sf::Time::Zero) {
		updateTime(m_emitTime, frameTime);
		if (m_emitTime == sf::Time::Zero) {
			m_pc->setEmitRate(0.f);
		}
	}
	if (m_hasDamaged) {
		m_data.damageType = DamageType::VOID;
	}
}

void HolyFireSpell::loadComponents() {
	// light
	LightData lightData(sf::Vector2f(m_data.range, m_data.range), m_data.range * 2.f, 0.5f);
	addComponent(new LightComponent(lightData, this));

	// particles
	ParticleComponentData data;
	data.particleCount = static_cast<int>(m_data.range);
	data.isAdditiveBlendMode = true;
	data.emitRate = m_data.range;
	data.texturePath = GlobalResource::TEX_PARTICLE_FLAME;

	// Generators
	auto spawner = new particles::DiskSpawner();
	spawner->radius = m_data.range;
	data.spawner = spawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 20.f;
	sizeGen->minEndSize = 20.f;
	sizeGen->maxEndSize = 60.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	if (m_data.skinNr == 1) {
		// shadow fire
		colGen->minStartCol = sf::Color(78, 60, 122, 50);
		colGen->maxStartCol = sf::Color(215, 181, 221, 100);
		colGen->minEndCol = sf::Color(20, 0, 30, 30);
		colGen->maxEndCol = sf::Color(25, 5, 35, 50);
	}
	else {
		// default holy fire
		colGen->minStartCol = sf::Color(162, 7, 0, 50);
		colGen->maxStartCol = sf::Color(234, 148, 0, 100);
		colGen->minEndCol = sf::Color(255, 244, 127, 30);
		colGen->maxEndCol = sf::Color(255, 255, 255, 50);
	}
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = -20.f;
	velGen->maxAngle = 20.f;
	velGen->minStartSpeed = 10.f;
	velGen->maxStartSpeed = 30.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 1.f;
	timeGen->maxTime = 1.2f;
	data.timeGen = timeGen;

	m_pc = new ParticleComponent(data, this);
	m_pc->setOffset(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f));
	addComponent(m_pc);
}
