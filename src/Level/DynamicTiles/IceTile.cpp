#include "Level/DynamicTiles/IceTile.h"
#include "Level/DynamicTiles/FluidTile.h"
#include "Spells/Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Ice, IceTile)

bool IceTile::init(const LevelTileProperties& properties) {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));

	m_isFreezing = contains(properties, std::string("freezing"));

	return true;
}

void IceTile::update(const sf::Time& frameTime) {
	if (!m_isFreezing) {
		LevelDynamicTile::update(frameTime);
		return;
	}

	for (auto it : *m_screen->getObjects(_DynamicTile)) {
		if (auto fluidTile = dynamic_cast<FluidTile*>(it)) {
			if (fastIntersect(*fluidTile->getBoundingBox(), m_boundingBox)) {
				int index = static_cast<int>(std::floor((getCenter().x - fluidTile->getPosition().x) / TILE_SIZE));
				fluidTile->freeze(index);
			}
		}
	}

	setDisposed();
	m_isFreezing = false;
}

void IceTile::loadAnimation(int skinNr) {
	m_isCollidable = true;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation(sf::seconds(0.5f));
	idleAnimation->setSpriteSheet(tex);
	for (int i = 0; i < 2; i++) {
		idleAnimation->addFrame(sf::IntRect(
			BORDER + i * (2 * BORDER + TILE_SIZE),
			BORDER + skinNr * (2 * BORDER + TILE_SIZE),
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

std::string IceTile::getSpritePath() const {
	return "res/texture/level_dynamic_tiles/spritesheet_tiles_ice.png";
}