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

ParticleTile::~ParticleTile() {
	delete m_ps;
}

void ParticleTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 50.f, 50.f));
	m_isCollidable = false;
}

void ParticleTile::loadAnimation(int skinNr_) {
	// initial values
	m_state = GameObjectState::Idle;
	int skinNr = skinNr_ / 2;
	m_isForegroundTile = skinNr_ % 2 != 0;

	loadParticleSystem(skinNr);
}

void ParticleTile::update(const sf::Time& frameTime) {
	// skip animation update, using game object only
	GameObject::update(frameTime);
	m_ps->update(frameTime);
}

void ParticleTile::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);

	if (m_particleSpawner == nullptr) return;
	m_particleSpawner->center = getPosition() + m_particlePosOffset;
	if (auto velGen = dynamic_cast<particles::AimedVelocityGenerator*>(m_velGenerator)) {
		velGen->goal = sf::Vector2f(getPosition().x + 0.5f * getBoundingBox()->width, getPosition().y - 10.f);
	}
}

void ParticleTile::render(sf::RenderTarget& target) {
	if (m_isFirstRenderIteration) {
		sf::RenderTarget& particleTarget = m_isForegroundTile ?
			dynamic_cast<LevelScreen*>(getScreen())->getParticleFGRenderTexture() :
			dynamic_cast<LevelScreen*>(getScreen())->getParticleBGRenderTexture();
		particleTarget.setView(target.getView());
		m_ps->render(particleTarget);
		GameObject::render(target);
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

void ParticleTile::loadWaterParticles(int skinNr) {
	m_particlePosOffset = sf::Vector2f(0.5f * getBoundingBox()->width, 0.8f * getBoundingBox()->height);
	g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_DROP)->setSmooth(true);
	m_ps = new particles::TextureParticleSystem(1000, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_DROP));
	m_ps->additiveBlendMode = false;
	m_ps->emitRate = 100.f;

	// Generators
	auto posGen = m_ps->addSpawner<particles::BoxSpawner>();
	posGen->center = getPosition() + m_particlePosOffset;
	posGen->size = sf::Vector2f(30.f, 20.f);
	m_particleSpawner = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 5.f;
	sizeGen->maxStartSize = 10.f;
	sizeGen->minEndSize = 10.f;
	sizeGen->maxEndSize = 20.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();

	colGen->minStartCol = sf::Color(60, 110, 40);
	colGen->maxStartCol = sf::Color(80, 100, 50);
	colGen->minEndCol = sf::Color(60, 80, 40, 100);
	colGen->maxEndCol = sf::Color(110, 140, 80, 100);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minStartSpeed = 30.f;
	velGen->maxStartSpeed = 50.f;
	velGen->minAngle = 180.f;
	velGen->maxAngle = 180.f;
	m_velGenerator = velGen;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 1.0f;
	timeGen->maxTime = 2.0f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}

void ParticleTile::loadEmberParticles(int skinNr) {
	m_particlePosOffset = sf::Vector2f(0.5f * getBoundingBox()->width, 0.8f * getBoundingBox()->height);
	g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_DROP)->setSmooth(true);
	m_ps = new particles::TextureParticleSystem(50, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_STAR));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 5.f;

	// Generators
	auto posGen = m_ps->addSpawner<particles::DiskSpawner>();
	posGen->center = getPosition() + m_particlePosOffset;
	posGen->radius = 30.f;
	m_particleSpawner = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 5.f;
	sizeGen->maxStartSize = 10.f;
	sizeGen->minEndSize = 0.f;
	sizeGen->maxEndSize = 0.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();

	colGen->minStartCol = sf::Color(110, 255, 100);
	colGen->maxStartCol = sf::Color(110, 255, 100);
	colGen->minEndCol = sf::Color(20, 200, 0, 200);
	colGen->maxEndCol = sf::Color(20, 200, 0, 200);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minStartSpeed = 20.f;
	velGen->maxStartSpeed = 30.f;
	velGen->minAngle = -10.f;
	velGen->maxAngle = 10.f;
	m_velGenerator = velGen;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 4.0f;
	timeGen->maxTime = 6.0f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();

	// light
	addComponent(new LightComponent(LightData(
		sf::Vector2f(TILE_SIZE_F * 0.5f, -TILE_SIZE_F * 0.2f),
		sf::Vector2f(200.f, 300.f), 0.3f), this));
}

void ParticleTile::loadFlameParticles(int skinNr) {
	m_particlePosOffset = sf::Vector2f(0.5f * getBoundingBox()->width, 0.5f * getBoundingBox()->height);
	g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_FLAME)->setSmooth(true);
	m_ps = new particles::TextureParticleSystem(1000, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_FLAME));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 60.f;

	// Generators
	auto posGen = m_ps->addSpawner<particles::BoxSpawner>();
	posGen->center = getPosition() + m_particlePosOffset;
	posGen->size = sf::Vector2f(40.f, 0.f);
	m_particleSpawner = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 30.f;
	sizeGen->maxStartSize = 50.f;
	sizeGen->minEndSize = 30.f;
	sizeGen->maxEndSize = 50.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();

	switch (skinNr) {
	default:
	case 0:
		colGen->minStartCol = sf::Color(255, 160, 64);
		colGen->maxStartCol = sf::Color(255, 160, 64);
		colGen->minEndCol = sf::Color(255, 0, 0, 200);
		colGen->maxEndCol = sf::Color(255, 0, 0, 200);
		break;
	case 1:
		colGen->minStartCol = sf::Color(100, 146, 186);
		colGen->maxStartCol = sf::Color(100, 146, 186);
		colGen->minEndCol = sf::Color(20, 83, 255, 200);
		colGen->maxEndCol = sf::Color(20, 83, 255, 200);
		break;
	case 3:
		colGen->minStartCol = sf::Color(110, 230, 100);
		colGen->maxStartCol = sf::Color(110, 230, 100);
		colGen->minEndCol = sf::Color(20, 150, 10, 200);
		colGen->maxEndCol = sf::Color(20, 150, 10, 200);
		break;
	}

	auto velGen = m_ps->addGenerator<particles::AimedVelocityGenerator>();
	velGen->goal = sf::Vector2f(getPosition().x + 0.5f * getBoundingBox()->width, getPosition().y - 10.f);
	velGen->minStartSpeed = 40.f;
	velGen->maxStartSpeed = 80.f;
	m_velGenerator = velGen;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 0.3f;
	timeGen->maxTime = 0.8f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();

	// light
	addComponent(new LightComponent(LightData(
		sf::Vector2f(TILE_SIZE_F * 0.5f, -TILE_SIZE_F * 0.5f),
		sf::Vector2f(200.f, 250.f), 0.6f), this));
}

void ParticleTile::loadParticleSystem(int skinNr) {
	switch (skinNr) {
	case 0:
	case 1:
	case 3:
		loadFlameParticles(skinNr);
		break;
	case 2:
		loadWaterParticles(skinNr);
		break;
	case 4:
	default:
		loadEmberParticles(skinNr);
		break;
	}
}
