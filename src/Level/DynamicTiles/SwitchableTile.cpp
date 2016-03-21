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

	Animation* onAnimation = new Animation(sf::seconds(10.0f));
	onAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_switchable));
	onAnimation->addFrame(sf::IntRect(
		BORDER,
		BORDER + (skinNr - 1) * (2 * BORDER + TILE_SIZE),
		TILE_SIZE,
		TILE_SIZE));

	addAnimation(GameObjectState::On, onAnimation);

	Animation* offAnimation = new Animation(sf::seconds(10.0f));
	offAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_switchable));
	offAnimation->addFrame(sf::IntRect(
		BORDER + (2 * BORDER + TILE_SIZE),
		BORDER + (skinNr - 1) * (2 * BORDER + TILE_SIZE),
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
