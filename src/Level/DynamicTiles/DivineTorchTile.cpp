#include "Level/DynamicTiles/DivineTorchTile.h"
#include "Screens/LevelScreen.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "Spells/Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::DivineTorch, DivineTorchTile)

DivineTorchTile::DivineTorchTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {
}

bool DivineTorchTile::init(const LevelTileProperties& properties) {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setPositionOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
	loadComponents();
	return true;
}

void DivineTorchTile::loadAnimation(int skinNr) {
	bool isOn = skinNr % 2 == 0;
	skinNr = skinNr / 2;
	int height = TILE_SIZE;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation(sf::seconds(10.f));
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, skinNr * height, TILE_SIZE, TILE_SIZE));
	idleAnimation->setLooped(false);

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* burningAnimation = new Animation(sf::seconds(10.f));
	burningAnimation->setSpriteSheet(tex);
	burningAnimation->addFrame(sf::IntRect(0, skinNr * height, TILE_SIZE, TILE_SIZE));
	burningAnimation->setLooped(false);
	
	addAnimation(GameObjectState::Burning, burningAnimation);

	// initial values
	m_state = isOn ? GameObjectState::Burning : GameObjectState::Idle;
	m_lightComponent->setVisible(isOn);
	m_pc->setVisible(isOn);
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);
}

void DivineTorchTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::Light:
		if (m_state == GameObjectState::Idle) {
			setState(GameObjectState::Burning);
			m_pc->setVisible(true);
			m_lightComponent->setVisible(true);
		}
		break;
	default:
		break;
	}
}

void DivineTorchTile::setTarget(GameObject* target, bool force) {
	if (!force && m_target == target) return;
	m_target = target;
	auto ps = m_pc->getParticleSystem();
	ps->removeGenerators<particles::VelocityGenerator>();

	if (m_target) {
		m_velGen = ps->addGenerator<particles::AimedVelocityGenerator>();
		m_velGen->goal = m_target->getCenter();
		m_velGen->minStartSpeed = 100.f;
		m_velGen->maxStartSpeed = 120.f;
	}
	else {
		m_velGen = nullptr;
		auto velGen = ps->addGenerator<particles::AngledVelocityGenerator>();
		velGen->minAngle = 90.f;
		velGen->maxAngle = 270.f;
		velGen->minStartSpeed = 10.f;
		velGen->maxStartSpeed = 20.f;
	}
}

void DivineTorchTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);
	if (m_state == GameObjectState::Burning && m_velGen && m_target) {
		m_velGen->goal = m_target->getCenter();
	}
}

void DivineTorchTile::loadComponents() {
	// light
	m_lightComponent = new LightComponent(LightData(sf::Vector2f(TILE_SIZE_F / 2.f, TILE_SIZE_F / 2.f), sf::Vector2f(150.f, 150.f)), this);
	addComponent(m_lightComponent);

	// particles
	ParticleComponentData data;
	data.particleCount = 20;
	data.texturePath = GlobalResource::TEX_PARTICLE_BLOB;
	data.emitRate = 5.f;
	data.isAdditiveBlendMode = true;
	data.particleTexture = &dynamic_cast<LevelScreen*>(m_screen)->getParticleBGRenderTexture();

	auto spawner = new particles::BoxSpawner();
	spawner->size = sf::Vector2f(getBoundingBox()->width * 0.2f, 0.f);
	data.spawner = spawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 30.f;
	sizeGen->minEndSize = 5.f;
	sizeGen->maxEndSize = 10.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = sf::Color(255, 255, 204, 100);
	colGen->maxStartCol = sf::Color(255, 255, 255, 150);
	colGen->minEndCol = sf::Color(255, 255, 0, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);
	data.colorGen = colGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 2.0f;
	timeGen->maxTime = 4.0f;
	data.timeGen = timeGen;

	m_pc = new ParticleComponent(data, this);
	m_pc->setOffset(sf::Vector2f(getBoundingBox()->width * 0.5f, 0.f));
	addComponent(m_pc);
	setTarget(nullptr, true);
}

std::string DivineTorchTile::getSpritePath() const {
	return "res/texture/level_dynamic_tiles/spritesheet_tiles_divinetorch.png";
}
