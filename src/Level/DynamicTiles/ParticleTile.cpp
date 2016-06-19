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

void ParticleTile::loadAnimation(int skinNr) {
	// initial values
	m_state = GameObjectState::Idle;

	if (skinNr == 1 || skinNr == 2) {
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

	if (m_particleSpawner == nullptr) return;
	m_particleSpawner->center.x = getPosition().x + 0.5f * getBoundingBox()->width;
	m_particleSpawner->center.y = getPosition().y + 0.5f * getBoundingBox()->height;
	m_velGenerator->goal = sf::Vector2f(getPosition().x + 0.5f * getBoundingBox()->width, getPosition().y - 10.f);
}

void ParticleTile::render(sf::RenderTarget& target) {
	if (m_isFirstRenderIteration) {
		sf::RenderTarget& particleTarget = dynamic_cast<LevelScreen*>(getScreen())->getParticleRenderTexture();
		particleTarget.setView(target.getView());
		m_ps->render(particleTarget);
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
	g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_FLAME)->setSmooth(true);
	m_ps = new particles::TextureParticleSystem(1000, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_FLAME));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 60.f;

	// Generators
	auto posGen = m_ps->addSpawner<particles::BoxSpawner>();
	posGen->center = sf::Vector2f(getPosition().x + 0.5f * getBoundingBox()->width, getPosition().y + 0.5f * getBoundingBox()->height);
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
	case 1:
		colGen->minStartCol = sf::Color(255, 160, 64);
		colGen->maxStartCol = sf::Color(255, 160, 64);
		colGen->minEndCol = sf::Color(255, 0, 0, 200);
		colGen->maxEndCol = sf::Color(255, 0, 0, 200);
		break;
	case 2:
		colGen->minStartCol = sf::Color(100, 146, 186);
		colGen->maxStartCol = sf::Color(100, 146, 186);
		colGen->minEndCol = sf::Color(20, 83, 255, 200);
		colGen->maxEndCol = sf::Color(20, 83, 255, 200);
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
}