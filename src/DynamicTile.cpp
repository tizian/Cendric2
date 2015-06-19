#include "DynamicTile.h"

void DynamicTile::update(const sf::Time& frameTime)
{
	GameObject::update(frameTime);
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

sf::Color DynamicTile::getConfiguredDebugColor() const
{
	return sf::Color::Yellow;
}

void DynamicTile::onHit(LevelMovableGameObject* mob)
{
	// nop
}
