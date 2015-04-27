#include "DynamicTiles\WaterTile.h"
#include "Spell.h"

void WaterTile::load()
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, static_cast<float>(m_tileSize.x), static_cast<float>(m_tileSize.y)));
	m_isCollidable = false;

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_water));
	idleAnimation.addFrame(sf::IntRect(BORDER + 1 * (2 * BORDER + m_tileSize.x), BORDER, m_tileSize.x, m_tileSize.y));
	idleAnimation.addFrame(sf::IntRect(BORDER + 2 * (2 * BORDER + m_tileSize.x), BORDER, m_tileSize.x, m_tileSize.y));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation frozenAnimation;
	frozenAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_water));
	frozenAnimation.addFrame(sf::IntRect(BORDER, BORDER, m_tileSize.x, m_tileSize.y));

	addAnimation(GameObjectState::Frozen, frozenAnimation);

	setFrameTime(sf::seconds(0.4f));

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);
}

void WaterTile::onHit(Spell* spell)
{
	switch (spell->getConfiguredType())
	{
	case SpellID::Ice:
		if (m_state == GameObjectState::Idle)
		{
			m_state = GameObjectState::Frozen;
			setCurrentAnimation(getAnimation(m_state), false);
			m_isCollidable = true;
			spell->setDisposed();
		}
		break;
	case SpellID::Fire:
		if (m_state == GameObjectState::Frozen)
		{
			m_state = GameObjectState::Idle;
			setCurrentAnimation(getAnimation(m_state), false);
			m_isCollidable = false;
			spell->setDisposed();
		}
		break;
	default:
		break;
	}
}