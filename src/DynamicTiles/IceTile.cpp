#include "DynamicTiles/IceTile.h"
#include "Spell.h"

void IceTile::init() {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, static_cast<float>(m_tileSize.x), static_cast<float>(m_tileSize.y)));
}

void IceTile::load(int skinNr) {
	m_isCollidable = true;

	Animation idleAnimation(sf::seconds(0.5f));
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_ice));
	for (int i = 0; i < 2; i++) {
		idleAnimation.addFrame(sf::IntRect(
			BORDER + i * (2 * BORDER + m_tileSize.x),
			BORDER + (skinNr - 1) * (2 * BORDER + m_tileSize.y),
			m_tileSize.x,
			m_tileSize.y));
	}

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

void IceTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::FireBall:
		spell->setDisposed();
		setDisposed();
		break;
	default:
		break;
	}
}