#include "Spells/InvisibilitySpell.h"
#include "LevelMainCharacter.h"

const sf::Time InvisibilitySpell::SMOKE_DURATION = sf::seconds(1);

InvisibilitySpell::InvisibilitySpell() : Spell() {
	m_smokeDuration = SMOKE_DURATION;
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
		GameObject::updateTime(m_smokeDuration, frameTime);
	}

	GameObject::updateTime(m_duration, frameTime);
	if (m_duration <= sf::Time::Zero) {
		setDisposed();
		m_mainChar->setInvisibilityLevel(0);
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
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(50, g_resourceManager->getTexture(ResourceID::Texture_Particle_smoke)));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 50.f;

	// Generators
	auto posGen = m_ps->addGenerator<particles::PointPositionGenerator>();
	posGen->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	m_pointGenerator = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 5.f;
	sizeGen->maxStartSize = 20.f;
	sizeGen->minEndSize = 10.f;
	sizeGen->maxEndSize = 50.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(24, 21, 57, 100);
	colGen->maxStartCol = sf::Color(51, 51, 71, 150);
	colGen->minEndCol = sf::Color(166, 167, 198, 0);
	colGen->maxEndCol = sf::Color(198, 199, 210, 50);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = -45.f;
	velGen->maxAngle = 45.f;
	velGen->minStartVel = 50.f;
	velGen->maxStartVel = 100.f;
	m_velGenerator = velGen;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 0.5f;
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