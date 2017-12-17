#include "Spells/ExplosionSpell.h"
#include "GlobalResource.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "World/CustomParticleUpdaters.h"
#include "Level/Enemies/VeliusBoss.h"

ExplosionSpell::ExplosionSpell() : Spell() {
}

void ExplosionSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	loadComponents();

	m_particleTime = sf::seconds(0.2f);
	m_lightTime = bean.activeDuration;
	m_iteration = 0;

	if (m_data.strength == 0) {
		DamageOverTimeData dot;
		dot.damageType = m_data.damageType;
		dot.duration = m_data.duration;
		dot.damage = m_data.damagePerSecond;
		m_mainChar->addDamageOverTime(dot);
	}

	m_data.damageType = DamageType::VOID;
}

void ExplosionSpell::update(const sf::Time& frameTime) {
	Spell::update(frameTime);

	m_lc->setBrightness(m_data.activeDuration.asSeconds() / m_lightTime.asSeconds());

	if (m_iteration == 3) return;

	updateTime(m_particleTime, frameTime);

	if (m_particleTime == sf::Time::Zero) {
		switch (m_iteration)
		{
		case 0:
		default:
			m_pc->setEmitRate(0.f);
			m_particleTime = sf::seconds(0.1f);
			m_iteration = 1;
			break;
		case 1: {
			m_pc->setEmitRate(4000.f);
			m_particleTime = sf::seconds(0.2f);
			auto colGen = m_pc->getColorGenerator();
			if (m_data.skinNr == 0) {
				colGen->minStartCol = sf::Color(255, 200, 100, 255);
				colGen->maxStartCol = sf::Color(255, 150, 100, 255);
				colGen->minEndCol = sf::Color(200, 0, 0, 200);
				colGen->maxEndCol = sf::Color(200, 100, 100, 200);
			}
			m_iteration = 2;
			break;
		}
		case 2:
			m_pc->setEmitRate(0.f);
			m_iteration = 3;
			break;
		}
	}
}

bool ExplosionSpell::getConfiguredRotateSprite() const {
	return false;
}

void ExplosionSpell::loadComponents() {
	// light
	LightData lightData(sf::Vector2f(m_boundingBox.height * 0.5f, m_boundingBox.height * 0.5f), 1000.f, 1.f);
	m_lc = new LightComponent(lightData, this);
	addComponent(m_lc);

	// particles
	ParticleComponentData data;
	data.texturePath = GlobalResource::TEX_PARTICLE_BEAM;
	data.isAdditiveBlendMode = true;
	data.emitRate = 4000.f;
	data.particleCount = 1000;

	auto posGen = new particles::DiskSpawner();
	posGen->radius = 10.f;
	data.spawner = posGen;
	if (m_data.strength == 0) {
		posGen->center = sf::Vector2f(675.f, 275.f);
	}

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 300.f;
	sizeGen->maxStartSize = 300.f;
	sizeGen->minEndSize = 500.f;
	sizeGen->maxEndSize = 500.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();

	switch (m_data.skinNr) {
	case 0:
	default:
		colGen->minStartCol = sf::Color(255, 100, 100, 255);
		colGen->maxStartCol = sf::Color(255, 100, 100, 255);
		colGen->minEndCol = sf::Color(255, 0, 0, 200);
		colGen->maxEndCol = sf::Color(255, 0, 0, 200);
		break;
	case 1:
		colGen->minStartCol = VeliusBoss::V_COLOR_TWILIGHT;
		colGen->maxStartCol = VeliusBoss::V_COLOR_TWILIGHT;
		colGen->minEndCol = VeliusBoss::V_COLOR_TWILIGHT;
		colGen->maxEndCol = VeliusBoss::V_COLOR_TWILIGHT;
		break;
	case 2:
		colGen->minStartCol = VeliusBoss::V_COLOR_NECROMANCY;
		colGen->maxStartCol = VeliusBoss::V_COLOR_NECROMANCY;
		colGen->minEndCol = VeliusBoss::V_COLOR_NECROMANCY;
		colGen->maxEndCol = VeliusBoss::V_COLOR_NECROMANCY;
		break;
	case 3:
		colGen->minStartCol = VeliusBoss::V_COLOR_DIVINE;
		colGen->maxStartCol = VeliusBoss::V_COLOR_DIVINE;
		colGen->minEndCol = VeliusBoss::V_COLOR_DIVINE;
		colGen->maxEndCol = VeliusBoss::V_COLOR_DIVINE;
		break;
	}
	
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartSpeed = 400.f;
	velGen->maxStartSpeed = 450.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 6.f;
	timeGen->maxTime = 6.f;
	data.timeGen = timeGen;

	m_pc = new  ParticleComponent(data, this);
	m_pc->setOffset(sf::Vector2f(getBoundingBox()->width * 0.5f, getBoundingBox()->height * 0.5f));
	m_pc->getParticleSystem()->addGenerator<particles::DirectionDefinedRotationGenerator>();
	
	if (m_data.strength > 0) {
		m_pc->getParticleSystem()->addUpdater(new particles::CollidingUpdater(m_level));
		m_pc->getParticleSystem()->addUpdater(new particles::KillingUpdater(m_mainChar));
	}
	else {
		m_pc->setStatic(true);
	}
	
	addComponent(m_pc);
}
