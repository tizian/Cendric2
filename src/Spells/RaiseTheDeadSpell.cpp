#include "Spells/RaiseTheDeadSpell.h"
#include "Level/Enemy.h"

RaiseTheDeadSpell::RaiseTheDeadSpell(int strength) : Spell() {
	m_strength = strength;
}

void RaiseTheDeadSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	loadParticleSystem();
}

void RaiseTheDeadSpell::update(const sf::Time& frameTime) {
	m_ps->update(frameTime);
	Spell::update(frameTime);
	updateParticleSystemPosition();
}

void RaiseTheDeadSpell::execOnHit(LevelMovableGameObject* target) {
	if (!target->isDead() || m_hasRisen) return;
	Enemy* enemy = dynamic_cast<Enemy*>(target);
	if (enemy == nullptr) return;
	if (enemy->getMentalStrength() > m_strength) {
		setDisposed();
		return;
	}
	AttributeData attributes = ZERO_ATTRIBUTES;
	attributes.damagePhysical = m_data.damage;
	attributes.damageFire = m_data.damage;
	attributes.damageIce = m_data.damage;
	attributes.damageLight = m_data.damage;
	attributes.damageShadow = m_data.damage;
	Enemy* copy = enemy->createNewControlledInstance(m_data.duration, attributes);
	m_screen->addObject(copy);
	m_hasRisen = true;
	setDisposed();
}

void RaiseTheDeadSpell::render(sf::RenderTarget& target) {
	Spell::render(target);
	m_ps->render(target);
}

void RaiseTheDeadSpell::loadParticleSystem() {
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(50, g_resourceManager->getTexture(ResourceID::Texture_Particle_blob2)));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 50.0f;

	// Generators
	auto posGen = m_ps->addGenerator<particles::BoxPositionGenerator>();
	posGen->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	posGen->size = sf::Vector2f(getBoundingBox()->width / 2.f, 0.f);
	m_pointGenerator = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 5.f;
	sizeGen->maxStartSize = 10.f;
	sizeGen->minEndSize = 1.f;
	sizeGen->maxEndSize = 3.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(31, 86, 28, 100);
	colGen->maxStartCol = sf::Color(44, 132, 57, 150);
	colGen->minEndCol = sf::Color(0, 255, 0, 0);
	colGen->maxEndCol = sf::Color(200, 255, 200, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartVel = 5.f;
	velGen->maxStartVel = 10.f;
	m_velGenerator = velGen;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 0.5f;
	timeGen->maxTime = 1.f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
}

void RaiseTheDeadSpell::updateParticleSystemPosition() {
	m_pointGenerator->center.x = getPosition().x + getBoundingBox()->width / 2;
	m_pointGenerator->center.y = getPosition().y + getBoundingBox()->height / 2;
}