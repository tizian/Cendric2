#include "Spells/TransformBeamSpell.h"
#include "GlobalResource.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/ParticleComponent.h"

TransformBeamSpell::TransformBeamSpell() : Spell() {
}

void TransformBeamSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	loadComponents();

	m_graceTime = sf::milliseconds(500);
}

void TransformBeamSpell::update(const sf::Time& frameTime) {
	Spell::update(frameTime);

	if (m_hasStunned)
		return;

	updateTime(m_graceTime, frameTime);

	if (m_graceTime == sf::Time::Zero) {
		m_pc->setEmitRate(0.f);
	}

	if (m_graceTime > sf::Time::Zero) {
		return;
	}

	bool isLeftOfMainChar = m_mainChar->getPosition().x > getPosition().x;
	if (m_mainChar->isFacingRight() != isLeftOfMainChar) {
		// he's looking into the direction of the spell, stun him (for eternity) and turn him into a wolf
		m_mainChar->setStunned(sf::seconds(1000.f));
		m_mainChar->setCurrentAnimation(m_mainChar->getAnimation(GameObjectState::Broken), !m_mainChar->isFacingRight(), true);
		m_hasStunned = true;
		// striptease 
		for (auto& go : *m_screen->getObjects(GameObjectType::_Equipment)) {
			go->setDisposed();
		}
	}
}

bool TransformBeamSpell::getConfiguredRotateSprite() const {
	return false;
}

void TransformBeamSpell::loadComponents() {
	// light
	LightData lightData(sf::Vector2f(m_boundingBox.width / 2.f, m_boundingBox.height / 2.f), 200.f, 0.8f);
	addComponent(new LightComponent(lightData, this));

	// particles
	ParticleComponentData data;
	data.texturePath = GlobalResource::TEX_PARTICLE_BEAM;
	data.isAdditiveBlendMode = true;
	data.emitRate = 120.f;
	data.particleCount = 400;

	auto posGen = new particles::DiskSpawner();
	posGen->radius = 10.f;
	data.spawner = posGen;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 200.f;
	sizeGen->maxStartSize = 500.f;
	sizeGen->minEndSize = 200.f;
	sizeGen->maxEndSize = 500.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = sf::Color(88, 129, 164, 100);
	colGen->maxStartCol = sf::Color(88, 129, 164, 100);
	colGen->minEndCol = sf::Color(16, 68, 209, 20);
	colGen->maxEndCol = sf::Color(16, 68, 209, 20);
	data.colorGen = colGen;

	sf::Vector2f direction = m_mainChar->getCenter() - getCenter();
	float angle = radToDeg(atan2(direction.y, direction.x) + M_PI * 0.5f);

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = angle - 40.f;
	velGen->maxAngle = angle + 40.f;
	velGen->minStartSpeed = 10.f;
	velGen->maxStartSpeed = 20.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 2.f;
	timeGen->maxTime = 2.f;
	data.timeGen = timeGen;

	m_pc = new  ParticleComponent(data, this);
	m_pc->setOffset(sf::Vector2f(getBoundingBox()->width * 0.5f, getBoundingBox()->height * 0.5f));
	m_pc->getParticleSystem()->addGenerator<particles::DirectionDefinedRotationGenerator>();
	addComponent(m_pc);
}
