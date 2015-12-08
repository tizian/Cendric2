#include "Level/DynamicTiles/CrumblyBlockTile.h"
#include "Spell.h"

void CrumblyBlockTile::init() {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, static_cast<float>(m_tileSize.x), static_cast<float>(m_tileSize.y)));
}

void CrumblyBlockTile::load(int skinNr) {
	m_isCollidable = true;

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_crumblyblock));
	idleAnimation.addFrame(sf::IntRect(BORDER, BORDER, m_tileSize.x, m_tileSize.y));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation crumblingAnimation;
	crumblingAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_crumblyblock));
	for (int i = 1; i < 5; i++) {
		crumblingAnimation.addFrame(sf::IntRect(
			BORDER + i * (2 * BORDER + m_tileSize.x), 
			BORDER + (skinNr - 1) * (2 * BORDER + m_tileSize.y), 
			m_tileSize.x, 
			m_tileSize.y));
	}

	addAnimation(GameObjectState::Crumbling, crumblingAnimation);

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
}

void CrumblyBlockTile::update(const sf::Time& frameTime) {
	if (m_state == GameObjectState::Crumbling) {
		m_crumblingTime = m_crumblingTime - frameTime;
		if (m_crumblingTime < sf::Time::Zero) {
			setDisposed();
		}
	}
	LevelDynamicTile::update(frameTime);
}

void CrumblyBlockTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::Chop:
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