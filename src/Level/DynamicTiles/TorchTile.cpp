#include "Level/DynamicTiles/TorchTile.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "Level/DynamicTiles/ParticleTile.h"
#include "Screens/LevelScreen.h"
#include "Spells/Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Torch, TorchTile)

TorchTile::TorchTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {
}

TorchTile::~TorchTile() {
	delete m_particleComponent;
}

bool TorchTile::init(const LevelTileProperties& properties) {
	setSpriteOffset(sf::Vector2f(-15.f, -TILE_SIZE_F / 2.f));
	setPositionOffset(sf::Vector2f(15.f, 0));
	setBoundingBox(sf::FloatRect(0.f, 0.f, 20.f, TILE_SIZE_F));

	m_color = "red";
	if (contains(properties, std::string("color"))) {
		m_color = properties.at("color");
	}

	return true;
}

void TorchTile::render(sf::RenderTarget& target) {
	if (m_isFirstRenderIteration) {
		m_particleComponent->render(target);
		AnimatedGameObject::render(target);
		m_isFirstRenderIteration = false;
	}
	else {
		m_isFirstRenderIteration = true;
	}
}

void TorchTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);
	m_particleComponent->update(frameTime);
}

void TorchTile::loadAnimation(int skinNr) {
	int textureHeight = 2 * TILE_SIZE;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, skinNr * textureHeight, TILE_SIZE, 2 * TILE_SIZE));
	idleAnimation->setLooped(false);

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* burningAnimation = new Animation();
	burningAnimation->setSpriteSheet(tex);
	burningAnimation->addFrame(sf::IntRect(0, skinNr * textureHeight, TILE_SIZE, 2 * TILE_SIZE));
	burningAnimation->setLooped(false);

	addAnimation(GameObjectState::Burning, burningAnimation);

	// initial values
	m_state = GameObjectState::Burning;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(false);

	loadComponents();
}

void TorchTile::loadComponents() {
	m_lightComponent = new LightComponent(LightData(sf::Vector2f(TILE_SIZE_F / 4.f, TILE_SIZE_F / 4.f), sf::Vector2f(140.f, 200.f)), this);
	addComponent(m_lightComponent);

	ParticleComponentData data;
	data.particleCount = 40;
	data.emitRate = 40.f;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_FLAME;

	// Generators
	auto posGen = new particles::BoxSpawner();
	posGen->size = sf::Vector2f(10.f, 0.f);
	data.spawner = posGen;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 20.f;
	sizeGen->maxStartSize = 30.f;
	sizeGen->minEndSize = 10.f;
	sizeGen->maxEndSize = 20.f;
	data.sizeGen = sizeGen;

	data.colorGen = ParticleTile::getFlameColorGenerator(m_color);

	m_velGen = new particles::AimedVelocityGenerator();
	m_velGen->goal = sf::Vector2f(getPosition().x + 0.5f * getBoundingBox()->width, getPosition().y - 10.f);
	m_velGen->minStartSpeed = 20.f;
	m_velGen->maxStartSpeed = 40.f;
	data.velGen = m_velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 0.5f;
	timeGen->maxTime = 1.0f;
	data.timeGen = timeGen;

	m_particleComponent = new ParticleComponent(data, this);
	m_particleComponent->setOffset(sf::Vector2f(0.5f * getBoundingBox()->width, 0.5f * getBoundingBox()->height));
}

void TorchTile::setPosition(const sf::Vector2f& pos) {
	LevelDynamicTile::setPosition(pos);
	if (m_velGen) {
		m_particleComponent->setPosition(pos);
		m_velGen->goal = sf::Vector2f(getPosition().x + 0.5f * getBoundingBox()->width, getPosition().y - 10.f);
	}
}

void TorchTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::IceBall:
		spell->setDisposed();
	case SpellID::Chop:
	case SpellID::WindGust:
		if (m_state == GameObjectState::Burning) {
			setState(GameObjectState::Idle);
			m_lightComponent->setVisible(false);
			m_particleComponent->setEmitRate(0.f);
		}
		break;
	case SpellID::FireBall:
		if (m_state == GameObjectState::Idle) {
			setState(GameObjectState::Burning);
			m_lightComponent->setVisible(true);
			m_particleComponent->setEmitRate(40.f);
			spell->setDisposed();
		}
		break;
	default:
		break;
	}
}

const std::string& TorchTile::getColor() const {
	return m_color;
}

std::string TorchTile::getSpritePath() const {
	return "res/texture/level_dynamic_tiles/spritesheet_tiles_torch.png";
}
