#include "Spells/InvisibilitySpell.h"

InvisibilitySpell::InvisibilitySpell() : Spell() {
}

void InvisibilitySpell::load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	loadParticleSystem();
}

void InvisibilitySpell::update(const sf::Time& frameTime) {
	calculatePositionAccordingToMob(m_nextPosition);
	setPosition(m_nextPosition);

	MovableGameObject::update(frameTime);
	
	if (m_smokeDuration > sf::Time::Zero) {
		m_ps->update(frameTime);
		updateParticleSystemPosition();
		AnimatedGameObject::updateTime(m_smokeDuration, frameTime);
	}

	AnimatedGameObject::updateTime(m_duration, frameTime);
	if (m_duration <= sf::Time::Zero) {
		setDisposed();
	}
}

void InvisibilitySpell::render(sf::RenderTarget& target) {
	if (m_smokeDuration > sf::Time::Zero) {
		m_ps->render(target);
	}
}

const sf::Vector2f InvisibilitySpell::getConfiguredPositionOffset() const {
	return sf::Vector2f(-49.f, 0.f);
}

bool InvisibilitySpell::getConfiguredRotateSprite() const {
	return false;
}

void InvisibilitySpell::loadParticleSystem() {
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(500, g_resourceManager->getTexture(ResourceID::Texture_Particle_snowflake)));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 500.0f / 5.0f;

	// Generators
	auto posGen = m_ps->addGenerator<particles::BoxPositionGenerator>();
	posGen->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	posGen->size = sf::Vector2f(getBoundingBox()->width / 2.f, 0.f);
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
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartVel = 200.f;
	velGen->maxStartVel = 200.f;
	m_velGenerator = velGen;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 1.0f;
	timeGen->maxTime = 1.0f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
}

void InvisibilitySpell::updateParticleSystemPosition() {
	m_pointGenerator->center.x = getPosition().x + getBoundingBox()->width / 2;
	m_pointGenerator->center.y = getPosition().y + getBoundingBox()->height / 2;
}