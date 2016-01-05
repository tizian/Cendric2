#include "Level/DynamicTiles/SwitchableTile.h"
#include "Spell.h"

void SwitchableTile::init() {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, static_cast<float>(m_tileSize.x), static_cast<float>(m_tileSize.y)));
}

void SwitchableTile::setInitialState(bool on) {
	m_isCollidable = on;
	m_isStrictlyCollidable = on;
}

void SwitchableTile::load(int skinNr) {

	Animation onAnimation(sf::seconds(10.0f));
	onAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_switchable));
	onAnimation.addFrame(sf::IntRect(
		BORDER,
		BORDER + (skinNr - 1) * (2 * BORDER + m_tileSize.y),
		m_tileSize.x,
		m_tileSize.y));

	addAnimation(GameObjectState::On, onAnimation);

	Animation offAnimation(sf::seconds(10.0f));
	offAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_switchable));
	offAnimation.addFrame(sf::IntRect(
		BORDER + (2 * BORDER + m_tileSize.x),
		BORDER + (skinNr - 1) * (2 * BORDER + m_tileSize.y),
		m_tileSize.x,
		m_tileSize.y));

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
