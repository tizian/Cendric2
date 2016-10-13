#include "Level/DynamicTiles/DisappearingTile.h"
#include "Level/LevelMainCharacter.h"
#include "Spells/Spell.h"
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

	Animation* idleAnimation = new Animation(sf::seconds(10.f));
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, (skinNr - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE));
	idleAnimation->setLooped(false);

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);

	initCriticalTime(skinNr);
}

void DisappearingTile::initCriticalTime(int skinNr) {
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
		setSpriteColor(sf::Color(255, 255, 255, static_cast<int>(m_criticalTime.asSeconds() / m_startCriticalTime.asSeconds() * 255.f)), sf::seconds(1));
		if (m_criticalTime == sf::Time::Zero) {
			setDisposed();
		}
	}
	
	checkForMainCharacter();
	LevelDynamicTile::update(frameTime);
}

void DisappearingTile::checkForMainCharacter() {
	if (m_isTouched) return;
	if (m_mainChar->getBoundingBox()->intersects(m_checkBoundingBox)) {
		m_isTouched = true;
	}
}

void DisappearingTile::setPosition(const sf::Vector2f& pos) {
	LevelDynamicTile::setPosition(pos);
	m_checkBoundingBox.left = pos.x - 1.f;
	m_checkBoundingBox.top = pos.y - 1.f;
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
	return "res/assets/level_dynamic_tiles/spritesheet_tiles_disappearing.png";
}