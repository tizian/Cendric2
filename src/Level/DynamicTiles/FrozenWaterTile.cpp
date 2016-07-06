#include "Level/DynamicTiles/FrozenWaterTile.h"
#include "Screens/LevelScreen.h"
#include "Spells/Spell.h"

#include "Level/DynamicTiles/FluidTile.h"

// this tile is special and is not registered
FrozenWaterTile::FrozenWaterTile(FluidTile* fluidTile, int fluidTileIndex) : LevelDynamicTile(dynamic_cast<LevelScreen*>(fluidTile->getScreen())) {
	m_fluidTile = fluidTile;
	m_fluidTileIndex = fluidTileIndex;
	m_isRenderAfterObjects = true;
}

void FrozenWaterTile::init() {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, static_cast<float>(TILE_SIZE), TILE_SIZE_F));
}

void FrozenWaterTile::loadAnimation(int skinNr) {
	m_isCollidable = true;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation(sf::seconds(0.5f));
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(BORDER, BORDER, TILE_SIZE, TILE_SIZE));
	
	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

void FrozenWaterTile::setDisposed() {
	m_fluidTile->melt(m_fluidTileIndex);
	LevelDynamicTile::setDisposed();
}

void FrozenWaterTile::onHit(Spell* spell) {
	using std::cout; using std::endl;
	switch (spell->getSpellID()) {
	case SpellID::FireBall:
		spell->setDisposed();
		setDisposed();
		break;
	default:
		break;
	}
}

std::string FrozenWaterTile::getSpritePath() const {
	return "res/assets/level_dynamic_tiles/spritesheet_tiles_frozenwater.png";
}