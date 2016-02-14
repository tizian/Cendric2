#include "Level/DynamicTiles/DestructibleTile.h"
#include "Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Destructible, DestructibleTile)

void DestructibleTile::init() {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
}

void DestructibleTile::loadAnimation(int skinNr) {
	m_isCollidable = true;

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_destructible));
	idleAnimation->addFrame(sf::IntRect(BORDER, BORDER, TILE_SIZE, TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* crumblingAnimation = new Animation();
	crumblingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_destructible));
	for (int i = 1; i < 5; i++) {
		crumblingAnimation->addFrame(sf::IntRect(
			BORDER + i * (2 * BORDER + TILE_SIZE),
			BORDER + (skinNr - 1) * (2 * BORDER + TILE_SIZE),
			TILE_SIZE,
			TILE_SIZE));
	}

	addAnimation(GameObjectState::Crumbling, crumblingAnimation);

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
}

void DestructibleTile::update(const sf::Time& frameTime) {
	if (m_state == GameObjectState::Crumbling) {
		m_crumblingTime = m_crumblingTime - frameTime;
		if (m_crumblingTime < sf::Time::Zero) {
			setDisposed();
		}
	}
	LevelDynamicTile::update(frameTime);
}

void DestructibleTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::Chop:
	case SpellID::Projectile:
		if (m_state == GameObjectState::Idle) {
			m_state = GameObjectState::Crumbling;
			setCurrentAnimation(getAnimation(m_state), false);
			m_isCollidable = false;
			spell->setDisposed();
		}
		break;
	default:
		break;
	}
}