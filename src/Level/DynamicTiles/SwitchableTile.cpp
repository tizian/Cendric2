#include "Level/DynamicTiles/SwitchableTile.h"
#include "Spells/Spell.h"

// this tile is special and is not registered
void SwitchableTile::init() {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
}

void SwitchableTile::setInitialState(bool on) {
	m_isCollidable = on;
	m_isStrictlyCollidable = on;
}

void SwitchableTile::loadAnimation(int skinNr) {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* onAnimation = new Animation(sf::seconds(10.0f));
	onAnimation->setSpriteSheet(tex);
	onAnimation->addFrame(sf::IntRect(
		BORDER,
		BORDER + skinNr * (2 * BORDER + TILE_SIZE),
		TILE_SIZE,
		TILE_SIZE));

	addAnimation(GameObjectState::On, onAnimation);

	Animation* offAnimation = new Animation(sf::seconds(10.0f));
	offAnimation->setSpriteSheet(tex);
	offAnimation->addFrame(sf::IntRect(
		BORDER + (2 * BORDER + TILE_SIZE),
		BORDER + skinNr * (2 * BORDER + TILE_SIZE),
		TILE_SIZE,
		TILE_SIZE));

	addAnimation(GameObjectState::Off, offAnimation);

	// initial values
	setState(m_isCollidable ? GameObjectState::On : GameObjectState::Off);
	playCurrentAnimation(false);
}

void SwitchableTile::switchTile() {
	m_isCollidable = !m_isCollidable;
	m_isStrictlyCollidable = m_isCollidable;
	setState(m_isCollidable ? GameObjectState::On : GameObjectState::Off);
}

bool SwitchableTile::isSwitchable() const {
	if (m_state == GameObjectState::On) return true;
	WorldCollisionQueryRecord rec;
	rec.boundingBox = m_boundingBox;
	if (m_level->collidesWithMobs(rec) || m_level->collidesWithMovableTiles(rec)) {
		g_logger->logInfo("SwitchableTile", "Cannot turn the tile on as it would stuck a MOB or a movable tile!");
		return false;
	}
	return true;
}

std::string SwitchableTile::getSpritePath() const {
	return "res/assets/level_dynamic_tiles/spritesheet_tiles_switchable.png";
}