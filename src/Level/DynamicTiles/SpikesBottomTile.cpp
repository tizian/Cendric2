#include "Level/DynamicTiles/SpikesBottomTile.h"
#include "Spells/Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::SpikesBottom, SpikesBottomTile)

bool SpikesBottomTile::init(const LevelTileProperties& properties) {
	setPositionOffset(sf::Vector2f(5.f, 25.f));
	setSpriteOffset(sf::Vector2f(-5.f, -25.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, 40.f, 25.f));

	return true;
}

void SpikesBottomTile::loadAnimation(int skinNr) {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation(sf::seconds(10.0f));
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(false);
}

void SpikesBottomTile::onHit(LevelMovableGameObject* mob) {
	mob->setDead();
}

std::string SpikesBottomTile::getSpritePath() const {
	return "res/texture/level_dynamic_tiles/spritesheet_tiles_spikesbottom.png";
}