#include "Spells/WindGustSpell.h"

#define SPELL_OFFSET 10.f

WindGustSpell::WindGustSpell(int strength) : Spell() {
	m_strength = strength;
	m_pushAcceleration = 100.f * strength;
}

void WindGustSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	loadParticleSystem();
}

void WindGustSpell::update(const sf::Time& frameTime) {
	Spell::update(frameTime);
	m_ps->update(frameTime);
	updateParticleSystemPosition();
	if (m_hasDamaged) {
		m_data.damageType = DamageType::VOID;
	}
}

void WindGustSpell::render(sf::RenderTarget& target) {
	Spell::render(target);
	m_ps->render(target);
}

void WindGustSpell::execOnHit(LevelMovableGameObject* target) {
	m_hasDamaged = true;
	if (Enemy* enemy = dynamic_cast<Enemy*>(target)) {
		if (enemy->getMentalStrength() < m_strength) {
			enemy->setAccelerationX(m_mob->getIsFacingRight() ? 4 * m_pushAcceleration : -4 * m_pushAcceleration);
		}
	}
	else {
		target->setAccelerationX(m_mob->getIsFacingRight() ? m_pushAcceleration : -m_pushAcceleration);
	}
}


sf::Vector2f WindGustSpell::getConfiguredPositionOffset() const {
	return sf::Vector2f(10.f, 10.f);
}

bool WindGustSpell::getConfiguredRotateSprite() const {
	return false;
}

float WindGustSpell::getPushAcceleration() const {
	return m_pushAcceleration;
}

void WindGustSpell::loadParticleSystem() {
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(static_cast<int>(m_pushAcceleration * 2), g_resourceManager->getTexture(ResourceID::Texture_Particle_blob)));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = getBoundingBox()->width;

	// Generators
	auto posGen = m_ps->addGenerator<particles::BoxPositionGenerator>();
	posGen->center = sf::Vector2f(getPosition().x + SPELL_OFFSET, getPosition().y + getBoundingBox()->height / 2);
	posGen->size = sf::Vector2f(0.f, getBoundingBox()->height / 2.f);
	m_pointGenerator = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 2.f;
	sizeGen->maxStartSize = 6.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 1.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(210, 230, 250, 255);
	colGen->maxStartCol = sf::Color(170, 180, 230, 255);
	colGen->minEndCol = sf::Color(100, 100, 108, 0);
	colGen->maxEndCol = sf::Color(90, 160, 170, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = 90 + -20.f;
	velGen->maxAngle = 90 + 20.f;
	velGen->minStartVel = m_pushAcceleration;
	velGen->maxStartVel = m_pushAcceleration;
	m_velGenerator = velGen;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 1.0f;
	timeGen->maxTime = getBoundingBox()->width / velGen->maxStartVel;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
}

void WindGustSpell::updateParticleSystemPosition() {
	if (!m_mob->getIsFacingRight()) {
		m_velGenerator->minAngle = -90 + -20.f;
		m_velGenerator->maxAngle = -90 + 20.f;
		m_pointGenerator->center.x = getPosition().x + getBoundingBox()->width - SPELL_OFFSET;
		m_pointGenerator->center.y = getPosition().y + getBoundingBox()->height / 2;
	}
	else {
		m_velGenerator->minAngle = 90 + -20.f;
		m_velGenerator->maxAngle = 90 + 20.f;
		m_pointGenerator->center.x = getPosition().x + SPELL_OFFSET;
		m_pointGenerator->center.y = getPosition().y + getBoundingBox()->height / 2;
	}
}