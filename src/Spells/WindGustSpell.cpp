#include "Spells/WindGustSpell.h"

#define SPELL_OFFSET 10.f

WindGustSpell::WindGustSpell() : Spell()
{
}

void WindGustSpell::load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target)
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	Spell::load(bean, mob, target);
	loadParticleSystem();
}

void WindGustSpell::update(const sf::Time& frameTime)
{
	Spell::update(frameTime);
	m_ps->update(frameTime);
	updateParticleSystemPosition();
}

void WindGustSpell::render(sf::RenderTarget& target)
{
	Spell::render(target);
	m_ps->render(target);
}

const sf::Vector2f WindGustSpell::getConfiguredPositionOffset() const
{
	return sf::Vector2f(10.f, 20.f);
}

bool WindGustSpell::getConfiguredRotateSprite() const
{
	return false;
}

void WindGustSpell::loadParticleSystem()
{
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(500, g_resourceManager->getTexture(ResourceID::Texture_Particle_blob)));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 500.0f / 5.0f;

	// Generators
	auto posGen = m_ps->addGenerator<particles::BoxPositionGenerator>();
	posGen->center = sf::Vector2f(getPosition().x + SPELL_OFFSET, getPosition().y + getBoundingBox()->height / 2);
	posGen->size = sf::Vector2f(0.f, getBoundingBox()->height / 2.f);
	m_pointGenerator = posGen.get();

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
	velGen->minStartVel = 200.f;
	velGen->maxStartVel = 200.f;
	m_velGenerator = velGen.get();

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 1.0f;
	timeGen->maxTime = getBoundingBox()->width / velGen->maxStartVel;

	// Updaters
	auto timeUpdater = m_ps->addUpdater<particles::TimeUpdater>();

	auto colorUpdater = m_ps->addUpdater<particles::ColorUpdater>();

	auto eulerUpdater = m_ps->addUpdater<particles::EulerUpdater>();
}

void WindGustSpell::updateParticleSystemPosition()
{
	if (!m_mob->getIsFacingRight())
	{
		m_velGenerator->minAngle = -90 + -20.f;
		m_velGenerator->maxAngle = -90 + 20.f;
		m_pointGenerator->center.x = getPosition().x + getBoundingBox()->width - SPELL_OFFSET;
		m_pointGenerator->center.y = getPosition().y + getBoundingBox()->height / 2;
	}
	else
	{
		m_velGenerator->minAngle = 90 + -20.f;
		m_velGenerator->maxAngle = 90 + 20.f;
		m_pointGenerator->center.x = getPosition().x + SPELL_OFFSET;
		m_pointGenerator->center.y = getPosition().y + getBoundingBox()->height / 2;
	}
}