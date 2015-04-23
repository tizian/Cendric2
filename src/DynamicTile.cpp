#include "DynamicTile.h"

void DynamicTile::update(sf::Time& frameTime)
{
	GameObject::update(frameTime);
}

bool DynamicTile::getIsCollidable()
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
