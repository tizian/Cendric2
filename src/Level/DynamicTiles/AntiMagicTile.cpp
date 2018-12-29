#include "Level/DynamicTiles/AntiMagicTile.h"
#include "Spells/Spell.h"
#include "Registrar.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "GameObjectComponents/LightComponent.h"
#include "World/CustomParticleUpdaters.h"
#include "Screens/LevelScreen.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::AntiMagic, AntiMagicTile)

bool AntiMagicTile::init(const LevelTileProperties& properties) {
	setSpriteOffset(sf::Vector2f(10.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));

	if (!contains(properties, std::string("width"))) {
		return false;
	}

	m_width = static_cast<float>(std::stoi(properties.at("width")));
	loadParticleSystem();
	auto& bb = *getBoundingBox();
	addComponent(new LightComponent(LightData(sf::Vector2f(bb.width * 0.5f, bb.height * 0.5f), sf::Vector2f(150.f, 100.f), 0.8f), this));
	addComponent(new LightComponent(LightData(sf::Vector2f(bb.width * 0.5f + m_width, bb.height * 0.5f), sf::Vector2f(150.f, 100.f), 0.8f), this));
	return true;
}

void AntiMagicTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);
	sf::Vector2f intersection;
	for (auto it : *m_screen->getObjects(_Spell)) {
		if (it->getBoundingBox()->contains(m_startPos) || World::lineBoxIntersection(m_startPos, m_endPos, *it->getBoundingBox(), intersection)) {
			it->setDisposed();
		}
	}
}

void AntiMagicTile::render(sf::RenderTarget& target) {
	target.draw(m_otherSprite);
	LevelDynamicTile::render(target);
}

void AntiMagicTile::setPosition(const sf::Vector2f& pos) {
	LevelDynamicTile::setPosition(pos);
	m_startPos = pos + sf::Vector2f(getBoundingBox()->width, getBoundingBox()->height * 0.5f);
	m_endPos = m_startPos + sf::Vector2f(m_width, 0.f);
	m_otherSprite.setPosition(pos + sf::Vector2f(m_width + 2 * TILE_SIZE_F - 10.f, 0.f));
	if (m_lineSpawner) {
		m_lineSpawner->point1 = m_startPos;
		m_lineSpawner->point2 = m_endPos;
	}
}

void AntiMagicTile::loadAnimation(int skinNr) {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation(sf::seconds(0.5f));
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));
	idleAnimation->setLooped(false);

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(false);

	m_otherSprite.setTexture(*tex);
	m_otherSprite.setTextureRect(sf::IntRect(0, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));
	m_otherSprite.setScale(sf::Vector2f(-1.f, 1.f));
}

std::string AntiMagicTile::getSpritePath() const {
	return "res/texture/level_dynamic_tiles/spritesheet_tiles_antimagic.png";
}

void AntiMagicTile::loadParticleSystem() {
	// line particles
	ParticleComponentData data;
	data.particleCount = static_cast<int>(m_width);
	data.emitRate = m_width * 2.f;
	data.isAdditiveBlendMode = true;
	data.texturePath = GlobalResource::TEX_PARTICLE_STAR;
	data.particleTexture = &dynamic_cast<LevelScreen*>(getScreen())->getParticleBGRenderTexture();

	// Generators
	m_lineSpawner = new particles::LineSpawner();
	data.spawner = m_lineSpawner;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 20.f;
	sizeGen->minEndSize = 10.f;
	sizeGen->maxEndSize = 10.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = COLOR_MEDIUM_PURPLE;
	colGen->maxStartCol = COLOR_MEDIUM_PURPLE;
	colGen->minEndCol = COLOR_MEDIUM_PURPLE;
	colGen->maxEndCol = COLOR_MEDIUM_PURPLE;
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minStartSpeed = 0.f;
	velGen->maxStartSpeed = 0.f;
	velGen->minAngle = 0.f;
	velGen->maxAngle = 0.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 0.4f;
	timeGen->maxTime = 0.6f;
	data.timeGen = timeGen;

	auto pc = new ParticleComponent(data, this);
	addComponent(pc);
}