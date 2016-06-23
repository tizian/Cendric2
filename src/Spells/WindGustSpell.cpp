#include "Spells/WindGustSpell.h"
#include "GlobalResource.h"

WindGustSpell::~WindGustSpell() {
	delete m_ps;
}

void WindGustSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	m_damageType = m_data.damageType;
	m_data.damageType = DamageType::VOID;
	m_pushAcceleration = 100.f * m_data.strength;
	loadParticleSystem();
}

void WindGustSpell::update(const sf::Time& frameTime) {
	Spell::update(frameTime);
	updateParticleSystemPosition();
	m_ps->update(frameTime);
	updateTime(m_timeUntilDamage, frameTime);
	if (m_timeUntilDamage == sf::Time::Zero) {
		m_data.damageType = m_damageType;
		m_timeUntilDamage = sf::seconds(1.f);
	}
	else {
		m_data.damageType = DamageType::VOID;
	}
}

void WindGustSpell::render(sf::RenderTarget& target) {
	Spell::render(target);
	m_ps->render(target);
}

void WindGustSpell::execOnHit(LevelMovableGameObject* target) {
	if (Enemy* enemy = dynamic_cast<Enemy*>(target)) {
		if (enemy->getMentalStrength() < m_data.ccStrength) {
			enemy->addAccelerationX(m_mob->isFacingRight() ? 4 * m_pushAcceleration : -4 * m_pushAcceleration);
		}
	}
	else {
		target->addAccelerationX(m_mob->isFacingRight() ? m_pushAcceleration : -m_pushAcceleration);
	}
}

bool WindGustSpell::getConfiguredRotateSprite() const {
	return false;
}

float WindGustSpell::getPushAcceleration() const {
	return m_pushAcceleration;
}

void WindGustSpell::loadParticleSystem() {
	m_ps = new particles::TextureParticleSystem(static_cast<int>((getBoundingBox()->width * getBoundingBox()->height) / 400.f * m_data.strength), g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_BLOB));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = getBoundingBox()->width / 5.f * m_data.strength;

	// Generators
	auto posGen = m_ps->addSpawner<particles::BoxSpawner>();
	posGen->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 10.f, getPosition().y + getBoundingBox()->height / 2);
	posGen->size = sf::Vector2f(getBoundingBox()->width / 5.f, getBoundingBox()->height * 0.5f);
	m_particleSpawner = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 4.f;
	sizeGen->maxStartSize = 12.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 2.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(210, 230, 250, 255);
	colGen->maxStartCol = sf::Color(170, 180, 230, 255);
	colGen->minEndCol = sf::Color(100, 100, 108, 0);
	colGen->maxEndCol = sf::Color(90, 160, 170, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = 90 + -20.f;
	velGen->maxAngle = 90 + 20.f;
	velGen->minStartSpeed = m_pushAcceleration;
	velGen->maxStartSpeed = m_pushAcceleration;
	m_velGenerator = velGen;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = getBoundingBox()->width / (velGen->maxStartSpeed * 3.f);
	timeGen->maxTime = getBoundingBox()->width / velGen->maxStartSpeed;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
}

void WindGustSpell::updateParticleSystemPosition() {
	if (m_mob == nullptr) return;
	if (!m_mob->isFacingRight()) {
		m_velGenerator->minAngle = -90 + -20.f;
		m_velGenerator->maxAngle = -90 + 20.f;
		m_particleSpawner->center.x = getPosition().x + getBoundingBox()->width - getBoundingBox()->width / 10.f;
		m_particleSpawner->center.y = getPosition().y + getBoundingBox()->height / 2;
	}
	else {
		m_velGenerator->minAngle = 90 + -20.f;
		m_velGenerator->maxAngle = 90 + 20.f;
		m_particleSpawner->center.x = getPosition().x + getBoundingBox()->width / 10.f;
		m_particleSpawner->center.y = getPosition().y + getBoundingBox()->height / 2;
	}
}