#include "Level/DynamicTiles/DestructibleTile.h"
#include "Spells/Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Destructible, DestructibleTile)

bool DestructibleTile::init(const LevelTileProperties& properties) {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));

	return true;
}

void DestructibleTile::loadAnimation(int skinNr) {
	m_isCollidable = true;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(BORDER, skinNr * (2 * BORDER + TILE_SIZE) + BORDER, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* crumblingAnimation = new Animation();
	crumblingAnimation->setSpriteSheet(tex);
	for (int i = 1; i < 5; i++) {
		crumblingAnimation->addFrame(sf::IntRect(
			BORDER + i * (2 * BORDER + TILE_SIZE),
			BORDER + skinNr * (2 * BORDER + TILE_SIZE),
			TILE_SIZE,
			TILE_SIZE));
	}
	crumblingAnimation->setLooped(false);

	addAnimation(GameObjectState::Crumbling, crumblingAnimation);

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
}

void DestructibleTile::update(const sf::Time& frameTime) {
	if (m_state == GameObjectState::Crumbling) {
		m_crumblingTime = m_crumblingTime - frameTime;
		if (m_crumblingTime < sf::Time::Zero) {
			setDisposed();
		}
	}
	LevelDynamicTile::update(frameTime);
}

void DestructibleTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::Chop:
	case SpellID::Projectile:
		if (m_state == GameObjectState::Idle) {
			m_state = GameObjectState::Crumbling;
			g_resourceManager->playSound(getSoundPath());
			setCurrentAnimation(getAnimation(m_state), false);
			m_isCollidable = false;
			spell->setDisposed();
		}
		break;
	default:
		break;
	}
}

std::string DestructibleTile::getSpritePath() const {
	return "res/texture/level_dynamic_tiles/spritesheet_tiles_destructible.png";
}

std::string DestructibleTile::getSoundPath() const {
	return "res/sound/tile/crumble.ogg";
}