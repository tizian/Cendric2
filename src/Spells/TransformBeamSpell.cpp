#include "Spells/TransformBeamSpell.h"
#include "GlobalResource.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/LightComponent.h"

TransformBeamSpell::TransformBeamSpell() : Spell() {
}

TransformBeamSpell::~TransformBeamSpell() {
	delete m_ps;
}

void TransformBeamSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	loadParticleSystem();

	LightData lightData(sf::Vector2f(m_boundingBox.width / 2.f, m_boundingBox.height / 2.f), 200.f, 0.8f);
	addComponent(new LightComponent(lightData, this));
}

void TransformBeamSpell::update(const sf::Time& frameTime) {
	Spell::update(frameTime);
	m_ps->update(frameTime);

	if (m_hasStunned)
		return;

	// this spell only hurts after a certain time that is dependant on the distance to the main char
	float distToMainChar = dist(m_mainChar->getCenter(), getCenter()); 
	m_timeSinceStart += frameTime;

	if (m_ps->emitRate > 0 && m_timeSinceStart > sf::seconds(0.5f)) {
		m_ps->emitRate = 0;
	}
	if (m_timeSinceStart.asSeconds() < distToMainChar / 400.f) // the constant is the velocity of the spells fastest particles
		return;

	bool isLeftOfMainChar = m_mainChar->getPosition().x > getPosition().x;
	if (m_mainChar->isFacingRight() ^ isLeftOfMainChar) {
		// he's looking into the direction of the spell, stun him (for eternity)
		m_mainChar->setStunned(sf::seconds(1000.f));
		m_hasStunned = true;
		// TODO: transform?
	}
}

void TransformBeamSpell::render(sf::RenderTarget& target) {
	Spell::render(target);
	m_ps->render(target);
}

bool TransformBeamSpell::getConfiguredRotateSprite() const {
	return false;
}

void TransformBeamSpell::loadParticleSystem() {
	sf::Texture* tex = g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_BEAM);
	tex->setSmooth(true);
	m_ps = new particles::TextureParticleSystem(400, tex);
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 120.f;

	// Generators
	auto posGen = m_ps->addSpawner<particles::DiskSpawner>();
	posGen->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2, getPosition().y + getBoundingBox()->height / 2);
	posGen->radius = 10.f;
	m_particleSpawner = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 200.f;
	sizeGen->maxStartSize = 500.f;
	sizeGen->minEndSize = 200.f;
	sizeGen->maxEndSize = 500.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(88, 129, 164, 100);
	colGen->maxStartCol = sf::Color(88, 129, 164, 100);
	colGen->minEndCol = sf::Color(16, 68, 209, 20);
	colGen->maxEndCol = sf::Color(16, 68, 209, 20);

	sf::Vector2f direction = m_mainChar->getCenter() - getCenter();
	float angle = radToDeg(atan2(direction.y, direction.x) + M_PI * 0.5f);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = angle - 40.f;
	velGen->maxAngle = angle + 40.f;
	velGen->minStartSpeed = 10.f;
	velGen->maxStartSpeed = 20.f;
	m_velGenerator = velGen;

	m_ps->addGenerator<particles::DirectionDefinedRotationGenerator>();

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 1.5f;
	timeGen->maxTime = 1.5f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
}
