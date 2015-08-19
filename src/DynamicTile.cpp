#include "DynamicTile.h"

DynamicTile::DynamicTile(Level* level) : GameObject()
{
	m_level = level;
}

bool DynamicTile::getIsCollidable() const
{
	return m_isCollidable;
}

void DynamicTile::setTileSize(const sf::Vector2i tileSize)
{
	m_tileSize = tileSize;
}

GameObjectType DynamicTile::getConfiguredType() const
{
	return GameObjectType::_DynamicTile;
}

void DynamicTile::onHit(LevelMovableGameObject* mob)
{
	// nop
}
