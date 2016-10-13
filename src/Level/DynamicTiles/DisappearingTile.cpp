#include "Level/DynamicTiles/DisappearingTile.h"
#include "Level/LevelMainCharacter.h"
#include "Spells/Spell.h"
#include "Screens/LevelScreen.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Disappearing, DisappearingTile)

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

	initForSkinNr(skinNr);
	loadParticleSystem();
}

void DisappearingTile::initForSkinNr(int skinNr) {
	switch (skinNr) {
	case 0:
		m_startCriticalTime = sf::seconds(0.25f);
		break;
	case 1:
		m_startCriticalTime = sf::seconds(0.5f);
		break;
	case 2:
		m_startCriticalTime = sf::seconds(1.f);
		break;
	case 3:
	default:
		m_startCriticalTime = sf::seconds(2.f);
		break;
	}

	m_criticalTime = m_startCriticalTime;
}

void DisappearingTile::update(const sf::Time& frameTime) {
	if (m_isTouched) {
		updateTime(m_criticalTime, frameTime);
		int newAlpha = static_cast<int>(m_criticalTime.asSeconds() / m_startCriticalTime.asSeconds() * 255.f);
		m_colorGenerator->maxStartCol.a = newAlpha;
		m_colorGenerator->minStartCol.a = newAlpha;
		if (m_criticalTime == sf::Time::Zero) {
			setDisposed();
		}
	}

	checkForMainCharacter();
	LevelDynamicTile::update(frameTime);
	m_ps->update(frameTime);
}

void DisappearingTile::render(sf::RenderTarget& target) {
	LevelDynamicTile::render(target);
	sf::RenderTarget& particleTarget = dynamic_cast<LevelScreen*>(getScreen())->getParticleRenderTexture();
	particleTarget.setView(target.getView());
	m_ps->render(particleTarget);
}

void DisappearingTile::checkForMainCharacter() {
	if (m_isTouched) return;
	if (m_mainChar->getBoundingBox()->intersects(m_checkBoundingBox)) {
		m_isTouched = true;
		m_ps->emitRate = 0.f;
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
	m_ps = new particles::TextureParticleSystem(100, g_resourceManager->getTexture(getSpritePath()));
	m_ps->emitRate = 20.f;

	// Generators
	auto posGen = m_ps->addSpawner<particles::BoxSpawner>();
	posGen->center = sf::Vector2f(getPosition().x + 0.5f * getBoundingBox()->width, getPosition().y + 0.5f * getBoundingBox()->height);
	posGen->size = sf::Vector2f(TILE_SIZE_F, TILE_SIZE_F);
	m_particleSpawner = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 30.f;
	sizeGen->minEndSize = 10.f;
	sizeGen->maxEndSize = 30.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = COLOR_WHITE;
	colGen->maxStartCol = COLOR_WHITE;
	colGen->minEndCol = sf::Color(255, 255, 255, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);
	m_colorGenerator = colGen;

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartSpeed = 5.f;
	velGen->maxStartSpeed = 10.f;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 3.f;
	timeGen->maxTime = 5.f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}