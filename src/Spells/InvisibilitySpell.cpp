#include "Spells/InvisibilitySpell.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"
#include "GlobalResource.h"

const sf::Time InvisibilitySpell::SMOKE_DURATION = sf::seconds(1);

InvisibilitySpell::InvisibilitySpell() : Spell() {
	m_smokeDuration = SMOKE_DURATION;
}

InvisibilitySpell::~InvisibilitySpell() {
	delete m_ps;
}

void InvisibilitySpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	loadParticleSystem();
}

void InvisibilitySpell::update(const sf::Time& frameTime) {
	sf::Vector2f nextPosition;
	calculatePositionAccordingToMob(nextPosition, m_mob);
	setPosition(nextPosition);

	MovableGameObject::update(frameTime);
	m_ps->update(frameTime);
	updateParticleSystemPosition();
	
	if (m_smokeDuration > sf::Time::Zero) {
		updateTime(m_smokeDuration, frameTime);
		if (m_smokeDuration == sf::Time::Zero)
			m_ps->emitRate = 0.f;
	}

	updateTime(m_data.activeDuration, frameTime);
	if (m_data.activeDuration <= sf::Time::Zero) {
		setDisposed();
	}
}

void InvisibilitySpell::setDisposed() {
	Spell::setDisposed();
	m_mainChar->setInvisibilityLevel(0);
}

void InvisibilitySpell::render(sf::RenderTarget& target) {
	m_ps->render(target);
}

void InvisibilitySpell::execOnHit(LevelMovableGameObject* target) {
	// nop
}

bool InvisibilitySpell::getConfiguredRotateSprite() const {
	return false;
}

void InvisibilitySpell::loadParticleSystem() {
	m_ps = new particles::TextureParticleSystem(100, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_SMOKE));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 50.f;

	// Generators
	auto spawner = m_ps->addSpawner<particles::DiskSpawner>();
	spawner->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	spawner->radius = 20.f;
	m_particleSpawner = spawner;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 80.f;
	sizeGen->minEndSize = 60.f;
	sizeGen->maxEndSize = 120.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(24, 21, 57, 100);
	colGen->maxStartCol = sf::Color(51, 51, 71, 150);
	colGen->minEndCol = sf::Color(166, 167, 198, 0);
	colGen->maxEndCol = sf::Color(198, 199, 210, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = -45.f;
	velGen->maxAngle = 45.f;
	velGen->minStartSpeed = 50.f;
	velGen->maxStartSpeed = 70.f;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 2.f;
	timeGen->maxTime = 2.f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}

void InvisibilitySpell::updateParticleSystemPosition() {
	m_particleSpawner->center.x = getPosition().x + getBoundingBox()->width / 2;
	m_particleSpawner->center.y = getPosition().y + getBoundingBox()->height / 2;
}