#include "DynamicTiles/WaterTile.h"
#include "Spell.h"

void WaterTile::init()
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, static_cast<float>(m_tileSize.x), static_cast<float>(m_tileSize.y)));
}

void WaterTile::load(int skinNr)
{
	m_isCollidable = false;

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_water));
	for (float f = 0.f; f < 1.f; f += 0.1f)
	{
		idleAnimation.addFrame(sf::IntRect(BORDER + static_cast<int>((1.f + f) * (2 * BORDER + m_tileSize.x)), BORDER, m_tileSize.x, m_tileSize.y));
	}

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation frozenAnimation;
	frozenAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_water));
	frozenAnimation.addFrame(sf::IntRect(BORDER, BORDER, m_tileSize.x, m_tileSize.y));

	addAnimation(GameObjectState::Frozen, frozenAnimation);

	setFrameTime(sf::seconds(0.2f));

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);
}

void WaterTile::onHit(Spell* spell)
{
	switch (spell->getConfiguredSpellID())
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