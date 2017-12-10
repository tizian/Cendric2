#include "Level/DynamicTiles/DisappearingTile.h"
#include "Level/LevelMainCharacter.h"
#include "Spells/Spell.h"
#include "GameObjectComponents/ParticleComponent.h"
#include "World/CustomParticleUpdaters.h"
#include "Screens/LevelScreen.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Disappearing, DisappearingTile)

const sf::Time DisappearingTile::RESPAWN_TIME = sf::seconds(5.f);

DisappearingTile::DisappearingTile(LevelScreen* levelScreen) :
	LevelDynamicTile(levelScreen) {
}

bool DisappearingTile::init(const LevelTileProperties& properties) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
	m_checkBoundingBox.width = TILE_SIZE_F + 2.f;
	m_checkBoundingBox.height = TILE_SIZE_F + 2.f;
	return true;
}

void DisappearingTile::loadAnimation(int skinNr) {
	m_isCollidable = true;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	// empty animation, this tile has a particle system
	Animation* idleAnimation = new Animation(sf::seconds(10.f));
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect());
	idleAnimation->setLooped(false);

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	m_skinNr = skinNr;
	initForSkinNr();
	loadComponents();
}

void DisappearingTile::initForSkinNr() {
	switch (m_skinNr) {
	case 0:
		m_skinColor = sf::Color(156, 145, 188, 200);
		m_criticalTime = sf::seconds(0.1f);
		break;
	case 1:
		m_skinColor = sf::Color(188, 145, 145);
		m_criticalTime = sf::seconds(0.5f);
		break;
	case 2:
		m_skinColor = sf::Color(150, 175, 200, 200);
		m_criticalTime = sf::seconds(1.f);
		break;
	case 3:
		m_skinColor = sf::Color(161, 188, 145, 200);
		m_criticalTime = sf::seconds(1.5f);
		break;
	case 4:
	default:
		m_skinColor = sf::Color(190, 145, 188, 200);
		m_criticalTime = sf::seconds(2.f);
		break;
	}
}

void DisappearingTile::update(const sf::Time& frameTime) {
	if (m_isTouched) {
		updateTime(m_criticalTime, frameTime);
		if (m_criticalTime == sf::Time::Zero) {
			m_isCollidable = false;
			m_respawnTime = RESPAWN_TIME;
			m_isTouched = false;
		}
	}

	if (m_respawnTime > sf::Time::Zero) {
		updateTime(m_respawnTime, frameTime);
		if (m_respawnTime == sf::Time::Zero) {
			respawn();
		}
	}

	checkForMainCharacter();
	LevelDynamicTile::update(frameTime);
}

void DisappearingTile::touch() {
	m_isTouched = true;
	m_pc->setEmitRate(0.f);
	dynamic_cast<particles::FadingColorUpdater*>(m_pc->getColorUpdater())->setFading(m_criticalTime.asSeconds());
}

void DisappearingTile::respawn() {
	// check for collidable
	WorldCollisionQueryRecord rec;
	rec.boundingBox = *getBoundingBox();
	if (m_level->collidesWithMobs(rec) || m_level->collidesWithMovableTiles(rec)) {
		g_logger->logInfo("DisappearingTile::respawn", "Cannot respawn this tile as it would stuck a MOB or a movable tile!");
		m_respawnTime = RESPAWN_TIME;
		return;
	}

	m_isCollidable = true;
	m_pc->setEmitRate(10.f);
	dynamic_cast<particles::FadingColorUpdater*>(m_pc->getColorUpdater())->resetColor();
	initForSkinNr();
}

void DisappearingTile::checkForMainCharacter() {
	if (m_isTouched) return;
	if (fastIntersect(*m_mainChar->getBoundingBox(), m_checkBoundingBox)) {
		touch();
	}
}

void DisappearingTile::setPosition(const sf::Vector2f& pos) {
	LevelDynamicTile::setPosition(pos);
	m_checkBoundingBox.left = pos.x - 1.f;
	m_checkBoundingBox.top = pos.y - 1.f;
}

void DisappearingTile::onHit(Spell* spell) {
	if (!m_isCollidable) return;
	switch (spell->getSpellID()) {
	case SpellID::Telekinesis:
		spell->setDisposed();
	case SpellID::WindGust:
	case SpellID::Chop:
		touch();
		break;
	default:
		break;
	}
}

std::string DisappearingTile::getSpritePath() const {
	return "res/texture/particles/cloud.png";
}

void DisappearingTile::loadComponents() {
	ParticleComponentData data;
	data.particleTexture = &(dynamic_cast<LevelScreen*>(getScreen())->getParticleFGRenderTexture());
	data.particleCount = 50;
	data.texturePath = getSpritePath();
	data.emitRate = 10.f;
	
	// Generators
	auto posGen = new particles::BoxSpawner();
	posGen->size = sf::Vector2f(TILE_SIZE_F, TILE_SIZE_F);
	data.spawner = posGen;

	auto sizeGen = new particles::SizeGenerator();
	sizeGen->minStartSize = 5.f;
	sizeGen->maxStartSize = 10.f;
	sizeGen->minEndSize = 30.f;
	sizeGen->maxEndSize = 40.f;
	data.sizeGen = sizeGen;

	auto colGen = new particles::ColorGenerator();
	colGen->minStartCol = m_skinColor;
	colGen->maxStartCol = m_skinColor;
	colGen->minEndCol = sf::Color(m_skinColor.r, m_skinColor.g, m_skinColor.b, 100);
	colGen->maxEndCol = sf::Color(m_skinColor.r, m_skinColor.g, m_skinColor.b, 100);
	data.colorGen = colGen;

	auto velGen = new particles::AngledVelocityGenerator();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartSpeed = 0.f;
	velGen->maxStartSpeed = 2.f;
	data.velGen = velGen;

	auto timeGen = new particles::TimeGenerator();
	timeGen->minTime = 4.f;
	timeGen->maxTime = 6.f;
	data.timeGen = timeGen;

	data.colorUpdater = new particles::FadingColorUpdater();
	m_pc = new ParticleComponent(data, this);
	m_pc->setOffset(sf::Vector2f(m_boundingBox.width * 0.5f, m_boundingBox.height * 0.5f));
	
	addComponent(m_pc);

	setPosition(getPosition());
	m_pc->simulate(sf::seconds(6.f));
}
