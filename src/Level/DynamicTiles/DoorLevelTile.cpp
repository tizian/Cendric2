#include "Level/DynamicTiles/DoorLevelTile.h"
#include "Spells/Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Door, DoorLevelTile)

void DoorLevelTile::init() {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
}

void DoorLevelTile::loadAnimation(int skinNr) {
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

void DoorLevelTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);
}

void DoorLevelTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	default:
		break;
	}
}

std::string DoorLevelTile::getSpritePath() const {
	return "res/assets/level_dynamic_tiles/spritesheet_tiles_destructible.png";
}
