#include "DynamicTile.h"

void DynamicTile::update(sf::Time& frameTime)
{
	GameObject::update(frameTime);
}

bool DynamicTile::isDisposed()
{
	return m_isDisposed;
}

void DynamicTile::setDisposed()
{
	m_isDisposed = true;
}

bool DynamicTile::getIsCollidable()
{
	return m_isCollidable;
}

void DynamicTile::setTileSize(const sf::Vector2i tileSize)
{
	m_tileSize = tileSize;
}
