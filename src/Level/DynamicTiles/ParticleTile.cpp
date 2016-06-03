#include "Level/DynamicTiles/ParticleTile.h"
#include "Level/LevelMainCharacter.h"
#include "Spells/Spell.h"
#include "Screens/LevelScreen.h"
#include "Registrar.h"
#include "GameObjectComponents/LightComponent.h"
#include "GlobalResource.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Particle, ParticleTile)

ParticleTile::ParticleTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {
}

void ParticleTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 50.f, 50.f));
	m_isCollidable = false;
}

void ParticleTile::loadAnimation(int skinNr) {
	// initial values
	m_state = GameObjectState::Idle;

	if (skinNr == 1) {
		addComponent(new LightComponent(LightData(
			sf::Vector2f(TILE_SIZE_F * 0.5f, -TILE_SIZE_F * 0.5f),
			sf::Vector2f(200.f, 250.f), 0.6f), this));
	}

	loadParticleSystem(skinNr);
}

void ParticleTile::update(const sf::Time& frameTime) {
	// skip animation update, using game object only
	GameObject::update(frameTime);
	m_ps->update(frameTime);
}

void ParticleTile::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);

	if (m_diskPosGenerator == nullptr) return;
	m_diskPosGenerator->center.x = getPosition().x + getBoundingBox()->width / 2;
	m_diskPosGenerator->center.y = getPosition().y + getBoundingBox()->height / 2;
}

void ParticleTile::render(sf::RenderTarget& target) {
	if (m_isFirstRenderIteration) {
		m_ps->render(target);
		m_isFirstRenderIteration = false;
	}
	else {
		m_isFirstRenderIteration = true;
	}
}

void ParticleTile::onHit(LevelMovableGameObject* mob) {
	// nop?
}

void ParticleTile::onHit(Spell* spell) {
	// nop?
}

void ParticleTile::loadParticleSystem(int skinNr) {
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(1000, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_STAR)));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 60.f;

	// Generators
	auto posGen = m_ps->addGenerator<particles::DiskPositionGenerator>();
	posGen->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	posGen->radius = 20.f;
	m_diskPosGenerator = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 5.f;
	sizeGen->maxStartSize = 10.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 1.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color::Yellow;
	colGen->maxStartCol = sf::Color::Red;
	colGen->minEndCol = sf::Color(0, 100, 20, 0);
	colGen->maxEndCol = sf::Color(100, 100, 20, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = -5.f;
	velGen->maxAngle = 5.f;
	velGen->minStartVel = 20.f;
	velGen->maxStartVel = 50.f;
	m_velGenerator = velGen;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 2.f;
	timeGen->maxTime = 3.f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}