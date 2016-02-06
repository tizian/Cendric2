#include "Level/DynamicTiles/IceTile.h"
#include "Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Ice, IceTile)

void IceTile::init() {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
}

void IceTile::loadAnimation(int skinNr) {
	m_isCollidable = true;

	Animation* idleAnimation = new Animation(sf::seconds(0.5f));
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_ice));
	for (int i = 0; i < 2; i++) {
		idleAnimation->addFrame(sf::IntRect(
			BORDER + i * (2 * BORDER + TILE_SIZE),
			BORDER + (skinNr - 1) * (2 * BORDER + TILE_SIZE),
			TILE_SIZE,
			TILE_SIZE));
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