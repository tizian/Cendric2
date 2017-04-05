#include "Spells/InvisibilitySpell.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "Screens/LevelScreen.h"
#include "GlobalResource.h"

const sf::Time InvisibilitySpell::SMOKE_DURATION = sf::seconds(1);

InvisibilitySpell::InvisibilitySpell() : Spell() {
	m_smokeDuration = SMOKE_DURATION;
}

void InvisibilitySpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	loadComponents();
}

void InvisibilitySpell::update(const sf::Time& frameTime) {
	sf::Vector2f nextPosition;
	calculatePositionAccordingToMob(nextPosition, m_mob);
	setPosition(nextPosition);

	MovableGameObject::update(frameTime);
	
	if (m_smokeDuration > sf::Time::Zero) {
		updateTime(m_smokeDuration, frameTime);
		if (m_smokeDuration == sf::Time::Zero)
			m_pc->setEmitRate(0.f);
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
	GameObject::render(target);
}

void InvisibilitySpell::execOnHit(LevelMovableGameObject* target) {
	// nop
}

bool InvisibilitySpell::getConfiguredRotateSprite() const {
	return false;
}

void InvisibilitySpell::loadComponents() {
	ParticleComponentData data;
	data.emitRate = 50.f;
	data.particleCount = 100;
	data.texturePath = GlobalResource::TEX_PARTICLE_SMOKE;
	data.isAdditiveBlendMode = true;
	
	auto spawner = new particles::DiskSpawner();
	spawner->radius = 20.f;
	data.spawner = spawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 80.f;
	sizeGen->minEndSize = 60.f;
	sizeGen->maxEndSize = 120.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = sf::Color(24, 21, 57, 100);
	colGen->maxStartCol = sf::Color(51, 51, 71, 150);
	colGen->minEndCol = sf::Color(166, 167, 198, 0);
	colGen->maxEndCol = sf::Color(198, 199, 210, 0);
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = -45.f;
	velGen->maxAngle = 45.f;
	velGen->minStartSpeed = 50.f;
	velGen->maxStartSpeed = 70.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 2.f;
	timeGen->maxTime = 2.f;
	data.timeGen = timeGen;

	m_pc = new ParticleComponent(data, this);
	m_pc->setOffset(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f));
	addComponent(m_pc);
}