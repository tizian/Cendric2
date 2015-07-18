#include "DynamicTiles/ChestTile.h"
#include "Spell.h"

void ChestTile::init()
{
	setBoundingBox(sf::FloatRect(0.f, 0.f, static_cast<float>(1.5 * m_tileSize.x), static_cast<float>(1.5 * m_tileSize.y)));
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
}

void ChestTile::load(int skinNr)
{
	m_isCollidable = false;

	Animation closedAnimation;
	closedAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_chest));
	closedAnimation.addFrame(sf::IntRect(0, 0, 2 * m_tileSize.x, 2 * m_tileSize.y));

	addAnimation(GameObjectState::Locked, closedAnimation);
	addAnimation(GameObjectState::Unlocked, closedAnimation);

	Animation openAnimation;
	openAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_chest));
	openAnimation.addFrame(sf::IntRect(2 * m_tileSize.x, 0, 2 * m_tileSize.x, 2 * m_tileSize.y));

	addAnimation(GameObjectState::Open, openAnimation);

	setFrameTime(sf::seconds(10.f));

	// initial values
	m_state = GameObjectState::Locked;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(false);
}

void ChestTile::onHit(Spell* spell)
{
	switch (spell->getSpellID())
	{
	case SpellID::Unlock:
		if (m_state == GameObjectState::Locked)
		{
			m_state = GameObjectState::Unlocked;
			setCurrentAnimation(getAnimation(m_state), false);
			spell->setDisposed();
		}
		break;
	default:
		break;
	}
}