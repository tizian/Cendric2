#include "Level/DynamicTiles/DisappearingTile.h"
#include "Spells/Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Disappearing, DisappearingTile)

DisappearingTile::DisappearingTile(LevelScreen* levelScreen) :
    LevelDynamicTile(levelScreen) {
}

void DisappearingTile::init() {
	setSpriteOffset(sf::Vector2f(-1.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, static_cast<float>(TILE_SIZE) - 2.f, TILE_SIZE_F));
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
	
	LevelDynamicTile::update(frameTime);
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

void DisappearingTile::onHit(LevelMovableGameObject* mob) {
	if (m_isTouched) return;
	if (mob->getConfiguredType() != GameObjectType::_LevelMainCharacter) return;
	m_isTouched = true;
}

std::string DisappearingTile::getSpritePath() const {
	return "res/assets/level_dynamic_tiles/spritesheet_tiles_disappearing.png";
}