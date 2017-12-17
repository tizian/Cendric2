#include "Spells/TargetingProjectileSpell.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/ParticleComponent.h"

void TargetingProjectileSpell::load(const SpellData& data, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	init(data);
	Spell::load(data, mob, target);
	loadComponents();
}

void TargetingProjectileSpell::init(const SpellData& data) {
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));

	Animation* spellAnimation = new Animation();
	spellAnimation->setSpriteSheet(g_resourceManager->getTexture(data.spritesheetPath));
	spellAnimation->addFrame(sf::IntRect(0, data.skinNr * 39, 39, 39));

	addAnimation(GameObjectState::Idle, spellAnimation);

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(false);
}

void TargetingProjectileSpell::update(const sf::Time& frameTime) {
	sf::Vector2f dir = m_mainChar->getCenter() - getPosition();
	float abs = norm(dir);
	dir = dir / abs;

	float velNorm = norm(m_velocity);
	auto absVel = m_velocity / velNorm;

	m_velocity = velNorm * normalized(absVel + m_data.strength * 0.01f * dir);

	Spell::update(frameTime);

	setSpriteRotation(atan2(getVelocity().y, getVelocity().x));
}

void TargetingProjectileSpell::loadComponents() {
	// light
	LightData lightData(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f), 80.f, 0.5f);
	addComponent(new LightComponent(lightData, this));

	// particles
	ParticleComponentData data;
	data.particleCount = 100;
	data.emitRate = 25.f;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_BLOB;
	
	// Generators
	auto spawner = new particles::BoxSpawner();
	spawner->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	spawner->size = sf::Vector2f(getBoundingBox()->width, 0.f);
	data.spawner = spawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 2.f;
	sizeGen->maxStartSize = 4.f;
	sizeGen->minEndSize = 10.f;
	sizeGen->maxEndSize = 20.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = sf::Color(210, 230, 250, 255);
	colGen->maxStartCol = sf::Color(170, 180, 230, 255);
	colGen->minEndCol = sf::Color(100, 100, 108, 0);
	colGen->maxEndCol = sf::Color(90, 160, 170, 0);
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartSpeed = 50.f;
	velGen->maxStartSpeed = 100.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 0.5f;
	timeGen->maxTime = 1.5f;
	data.timeGen = timeGen;

	auto pc = new ParticleComponent(data, this);
	pc->setOffset(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f));
	addComponent(pc);
}
