#include "Level/DynamicTiles/SpikesBottomTile.h"
#include "Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::SpikesBottom, SpikesBottomTile)

void SpikesBottomTile::init() {
	setPositionOffset(sf::Vector2f(-5.f, -25.f));
	setSpriteOffset(sf::Vector2f(-5.f, -25.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, 40.f, 25.f));
}

void SpikesBottomTile::load(int skinNr) {

	Animation idleAnimation(sf::seconds(10.0f));
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_spikesbottom));
	idleAnimation.addFrame(sf::IntRect(0, (skinNr - 1) * m_tileSize.y, m_tileSize.x, m_tileSize.y));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(false);
}

void SpikesBottomTile::onHit(LevelMovableGameObject* mob) {
	mob->setDead();
}