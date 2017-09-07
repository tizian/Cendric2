#include "Spells/WindGustSpell.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "GlobalResource.h"

void WindGustSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	m_damageType = m_data.damageType;
	m_data.damageType = DamageType::VOID;
	m_absPushAcceleration = 100.f * m_data.strength;
	m_pushAcceleration = m_absPushAcceleration;
	loadParticleSystem();
}

void WindGustSpell::update(const sf::Time& frameTime) {
	Spell::update(frameTime);
	updateTime(m_timeUntilDamage, frameTime);
	if (m_timeUntilDamage == sf::Time::Zero) {
		m_data.damageType = m_damageType;
		m_timeUntilDamage = sf::seconds(1.f);
	}
	else {
		m_data.damageType = DamageType::VOID;
	}
}

void WindGustSpell::setPosition(const sf::Vector2f& pos) {
	Spell::setPosition(pos);
	updateParticleSystemPosition();
}

void WindGustSpell::execOnHit(LevelMovableGameObject* target) {
	if (Enemy* enemy = dynamic_cast<Enemy*>(target)) {
		if (enemy->getMentalStrength() < m_data.ccStrength) {
			enemy->addAccelerationX(4 * m_pushAcceleration);
		}
	}
}

bool WindGustSpell::getConfiguredRotateSprite() const {
	return false;
}

float WindGustSpell::getPushAcceleration() const {
	return m_pushAcceleration;
}

void WindGustSpell::loadParticleSystem() {
	ParticleComponentData data;
	data.particleCount = static_cast<int>((getBoundingBox()->width * getBoundingBox()->height) / 400.f * m_data.strength);
	data.emitRate = getBoundingBox()->width / 5.f * m_data.strength;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_BLOB;
	
	// Generators
	m_particleSpawner = new particles::BoxSpawner();
	m_particleSpawner->size = sf::Vector2f(getBoundingBox()->width * 0.1f, getBoundingBox()->height * 0.5f);
	data.spawner = m_particleSpawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 4.f;
	sizeGen->maxStartSize = 12.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 2.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = sf::Color(210, 230, 250, 255);
	colGen->maxStartCol = sf::Color(170, 180, 230, 255);
	colGen->minEndCol = sf::Color(100, 100, 108, 0);
	colGen->maxEndCol = sf::Color(90, 160, 170, 0);
	data.colorGen = colGen;

	m_velGenerator = new particles::AngledVelocityGenerator();
	m_velGenerator->minAngle = 90 + -20.f;
	m_velGenerator->maxAngle = 90 + 20.f;
	m_velGenerator->minStartSpeed = m_pushAcceleration * 1.5f;
	m_velGenerator->maxStartSpeed = m_pushAcceleration * 1.5f;
	data.velGen = m_velGenerator;

	auto timeGen =  new particles::TimeGenerator();
	timeGen->minTime = getBoundingBox()->width / (m_velGenerator->maxStartSpeed * 3.f);
	timeGen->maxTime = getBoundingBox()->width / m_velGenerator->maxStartSpeed;
	data.timeGen = timeGen;

	m_pc = new ParticleComponent(data, this);
	addComponent(m_pc);
}

void WindGustSpell::updateParticleSystemPosition() {
	if (m_mob == nullptr) return;
	if (m_pc == nullptr) return;
	if (!m_mob->isFacingRight()) {
		m_pushAcceleration = -m_absPushAcceleration;
		m_velGenerator->minAngle = -90 + -20.f;
		m_velGenerator->maxAngle = -90 + 20.f;
		m_particleSpawner->center.x = getPosition().x + getBoundingBox()->width - getBoundingBox()->width * 0.1f;
		m_particleSpawner->center.y = getPosition().y + getBoundingBox()->height * 0.5f;
	}
	else {
		m_pushAcceleration = m_absPushAcceleration;
		m_velGenerator->minAngle = 90 + -20.f;
		m_velGenerator->maxAngle = 90 + 20.f;
		m_particleSpawner->center.x = getPosition().x + getBoundingBox()->width * 0.1f;
		m_particleSpawner->center.y = getPosition().y + getBoundingBox()->height * 0.5f;
	}
}