#include "Level/DynamicTiles/DisappearingTile.h"
#include "Level/LevelMainCharacter.h"
#include "Spells/Spell.h"
#include "Screens/LevelScreen.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Disappearing, DisappearingTile)

const sf::Time DisappearingTile::RESPAWN_TIME = sf::seconds(5.f);

DisappearingTile::DisappearingTile(LevelScreen* levelScreen) :
	LevelDynamicTile(levelScreen) {
}

void DisappearingTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
	m_checkBoundingBox.width = TILE_SIZE_F + 2.f;
	m_checkBoundingBox.height = TILE_SIZE_F + 2.f;
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
	loadParticleSystem();
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
	m_ps->update(frameTime);
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
	m_ps->emitRate = 10.f;
	m_colorUpdater->resetColor();
	initForSkinNr();
}

void DisappearingTile::render(sf::RenderTarget& target) {
	LevelDynamicTile::render(target);
	sf::RenderTarget& particleTarget = dynamic_cast<LevelScreen*>(getScreen())->getParticleFGRenderTexture();
	particleTarget.setView(target.getView());
	m_ps->render(particleTarget);
}

void DisappearingTile::checkForMainCharacter() {
	if (m_isTouched) return;
	if (m_mainChar->getBoundingBox()->intersects(m_checkBoundingBox)) {
		m_isTouched = true;
		m_ps->emitRate = 0.f;
		m_colorUpdater->setFading(m_criticalTime.asSeconds());
	}
}

void DisappearingTile::setPosition(const sf::Vector2f& pos) {
	LevelDynamicTile::setPosition(pos);
	m_checkBoundingBox.left = pos.x - 1.f;
	m_checkBoundingBox.top = pos.y - 1.f;

	if (m_particleSpawner == nullptr) return;
	m_particleSpawner->center.x = getPosition().x + 0.5f * getBoundingBox()->width;
	m_particleSpawner->center.y = getPosition().y + 0.5f * getBoundingBox()->height;
}

void DisappearingTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::WindGust:
	case SpellID::Chop:
	case SpellID::Telekinesis:
		m_isTouched = true;
		break;
	default:
		break;
	}
}

std::string DisappearingTile::getSpritePath() const {
	return "res/assets/particles/cloud.png";
}

void DisappearingTile::loadParticleSystem() {
	g_resourceManager->getTexture(getSpritePath())->setSmooth(true);
	m_ps = new particles::TextureParticleSystem(50, g_resourceManager->getTexture(getSpritePath()));
	m_ps->emitRate = 10.f;

	// Generators
	auto posGen = m_ps->addSpawner<particles::BoxSpawner>();
	posGen->center = sf::Vector2f(getPosition().x + 0.5f * getBoundingBox()->width, getPosition().y + 0.5f * getBoundingBox()->height);
	posGen->size = sf::Vector2f(TILE_SIZE_F, TILE_SIZE_F);
	m_particleSpawner = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 5.f;
	sizeGen->maxStartSize = 10.f;
	sizeGen->minEndSize = 30.f;
	sizeGen->maxEndSize = 40.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = m_skinColor;
	colGen->maxStartCol = m_skinColor;
	colGen->minEndCol = sf::Color(m_skinColor.r, m_skinColor.g, m_skinColor.b, 0);
	colGen->maxEndCol = sf::Color(m_skinColor.r, m_skinColor.g, m_skinColor.b, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartSpeed = 0.f;
	velGen->maxStartSpeed = 2.f;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 4.f;
	timeGen->maxTime = 6.f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_colorUpdater = m_ps->addUpdater<particles::FadingColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}
