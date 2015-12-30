#include "Spells/FlashSpell.h"

FlashSpell::FlashSpell() : Spell() {
}

void FlashSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	m_isFlashingRight = mob->getIsFacingRight();
	m_mob = mob;
	loadParticleSystem();
}

void FlashSpell::update(const sf::Time& frameTime) {
	m_ps->update(frameTime);
	Spell::update(frameTime);
	updateParticleSystemPosition();
}

void FlashSpell::render(sf::RenderTarget& target) {
	m_ps->render(target);
}

void FlashSpell::execOnHit(LevelMovableGameObject* target) {
	// this spell only hurts once
	m_damageType = DamageType::VOID;
}

void FlashSpell::loadParticleSystem() {
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(500, g_resourceManager->getTexture(ResourceID::Texture_Particle_longblob)));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 500.0f / 5.0f;

	// Generators
	auto posGen = m_ps->addGenerator<particles::BoxPositionGenerator>();
	posGen->center.x = m_mob->getPosition().x + getBoundingBox()->width / 2;
	posGen->center.y = m_mob->getPosition().y + getBoundingBox()->height / 2;
	posGen->size = sf::Vector2f(10.f, getBoundingBox()->height);
	m_posGenerator = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 2.f;
	sizeGen->maxStartSize = 6.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 1.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(255, 255, 204, 100);
	colGen->maxStartCol = sf::Color(255, 255, 255, 150);
	colGen->minEndCol = sf::Color(255, 255, 0, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = m_isFlashingRight ? 260.f : 80.f;
	velGen->maxAngle = m_isFlashingRight ? 280.f : 100.f;
	velGen->minStartVel = 200.f;
	velGen->maxStartVel = 400.f;
	m_velGenerator = velGen;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 0.1f;
	timeGen->maxTime = 0.4f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
}

void FlashSpell::updateParticleSystemPosition() {
	m_posGenerator->center.x = m_mob->getPosition().x + getBoundingBox()->width / 2;
	m_posGenerator->center.y = m_mob->getPosition().y + getBoundingBox()->height / 2;
}