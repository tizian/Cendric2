#include "Level/DynamicTiles/OnewayTile.h"
#include "Spells/Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Oneway, OnewayTile)

bool OnewayTile::init(const LevelTileProperties& properties) {
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
	m_isOneWay = true;
	m_isCollidable = true;
	return true;
}

void OnewayTile::loadAnimation(int skinNr) {
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation(sf::seconds(10.0f));
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, skinNr * TILE_SIZE, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(false);
}

std::string OnewayTile::getSpritePath() const {
	return "res/assets/level_dynamic_tiles/spritesheet_tiles_oneway.png";
}