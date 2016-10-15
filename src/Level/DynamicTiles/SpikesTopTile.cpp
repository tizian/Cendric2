#include "Level/DynamicTiles/SpikesTopTile.h"
#include "Spells/Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::SpikesTop, SpikesTopTile)

void SpikesTopTile::init() {
	setPositionOffset(sf::Vector2f(5.f, 40.f));
	setSpriteOffset(sf::Vector2f(-5.f, -40.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, 40.f, 40.f));
}

void SpikesTopTile::loadAnimation(int skinNr) {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation(sf::seconds(10.0f));
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, skinNr * TILE_SIZE * 2, TILE_SIZE, TILE_SIZE * 2));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(false);
}

void SpikesTopTile::onHit(LevelMovableGameObject* mob) {
	mob->setDead();
}

std::string SpikesTopTile::getSpritePath() const {
	return "res/assets/level_dynamic_tiles/spritesheet_tiles_spikestop.png";
}