#include "Map.h"

Map::Map()
{
}

Map::~Map()
{
	dispose();
}

void Map::dispose()
{
	m_tileMap.dispose();
}

bool Map::load(ResourceID id)
{
	MapReader reader;
	MapData data;
	if (!reader.readMap(g_resourceManager->getFilename(id), data))
	{
		return false;
	}

	// load map
	m_startPos = data.startPos;
	m_name = data.name;
	m_tileMap.load(data.tileSetPath, data.tileSize, data.layers, data.mapSize.x, data.mapSize.y);
	m_collidableTiles = data.collidableTileRects;
	m_mapRect = data.mapRect;
	return true;
}

void Map::draw(sf::RenderTarget &target, sf::RenderStates states, const sf::Vector2f& center)
{
	sf::View view;
	view.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	float camCenterX = std::max(WINDOW_WIDTH / 2.f, std::min(m_mapRect.width - WINDOW_WIDTH / 2.f, center.x));
	float camCenterY = std::max(WINDOW_HEIGHT / 2.f, std::min(m_mapRect.height - WINDOW_HEIGHT / 2.f, center.y));
	view.setCenter(camCenterX, camCenterY);
	target.setView(view);
	m_tileMap.draw(target, states);
}

sf::FloatRect& Map::getMapRect()
{
	return m_mapRect;
}

TileMap Map::getTilemap()
{
	return m_tileMap;
}

sf::Vector2f& Map::getStartPos()
{
	return m_startPos;
}

bool Map::collidesX(const sf::FloatRect& boundingBox)
{
	// check for collision with level rect
	if (boundingBox.left < m_mapRect.left || boundingBox.left + boundingBox.width > m_mapRect.left + m_mapRect.width)
	{
		return true;
	}

	float tileWidth = static_cast<float>(m_tileMap.getTilesize().x);
	float tileHeight = static_cast<float>(m_tileMap.getTilesize().y);

	// normalize bounding box values so they match our collision grid. Wondering about the next two lines? Me too. We just don't want to floor values that are exactly on the boundaries. But only those that are down and right.
	int bottomY = static_cast<int>(floor((boundingBox.top + boundingBox.height) / tileHeight) == (boundingBox.top + boundingBox.height) / tileHeight ? (boundingBox.top + boundingBox.height) / tileHeight - 1 : floor((boundingBox.top + boundingBox.height) / tileHeight));
	int rightX = static_cast<int>(floor((boundingBox.left + boundingBox.width) / tileWidth) == (boundingBox.left + boundingBox.width) / tileWidth ? (boundingBox.left + boundingBox.width) / tileWidth - 1 : floor((boundingBox.left + boundingBox.width) / tileWidth));
	sf::Vector2i topLeft(static_cast<int>(floor(boundingBox.left / tileWidth)), static_cast<int>(floor(boundingBox.top / tileHeight)));
	sf::Vector2i topRight(rightX, static_cast<int>(floor(boundingBox.top / tileHeight)));
	sf::Vector2i bottomLeft(static_cast<int>(floor(boundingBox.left / tileWidth)), bottomY);
	sf::Vector2i bottomRight(rightX, bottomY);

	// check left side
	int x = topLeft.x;
	for (int y = topLeft.y; y <= bottomLeft.y; y++)
	{
		if (m_collidableTiles[y][x])
		{
			return true;
		}
	}

	// check right side
	x = topRight.x;
	for (int y = topRight.y; y <= bottomRight.y; y++)
	{
		if (m_collidableTiles[y][x])
		{
			return true;
		}
	}

	return false;
}

bool Map::collidesY(const sf::FloatRect& boundingBox)
{
	// check for collision with level rect
	if (boundingBox.top < m_mapRect.top || boundingBox.top + boundingBox.height > m_mapRect.top + m_mapRect.height)
	{
		return true;
	}

	float tileWidth = static_cast<float>(m_tileMap.getTilesize().x);
	float tileHeight = static_cast<float>(m_tileMap.getTilesize().y);

	// normalize bounding box values so they match our collision grid. Wondering about the next two lines? Me too. We just don't want to floor values that are exactly on the boundaries. But only those that are down and right.
	int bottomY = static_cast<int>(floor((boundingBox.top + boundingBox.height) / tileHeight) == (boundingBox.top + boundingBox.height) / tileHeight ? (boundingBox.top + boundingBox.height) / tileHeight - 1 : floor((boundingBox.top + boundingBox.height) / tileHeight));
	int rightX = static_cast<int>(floor((boundingBox.left + boundingBox.width) / tileWidth) == (boundingBox.left + boundingBox.width) / tileWidth ? (boundingBox.left + boundingBox.width) / tileWidth - 1 : floor((boundingBox.left + boundingBox.width) / tileWidth));
	sf::Vector2i topLeft(static_cast<int>(floor(boundingBox.left / tileWidth)), static_cast<int>(floor(boundingBox.top / tileHeight)));
	sf::Vector2i topRight(rightX, static_cast<int>(floor(boundingBox.top / tileHeight)));
	sf::Vector2i bottomLeft(static_cast<int>(floor(boundingBox.left / tileWidth)), bottomY);
	sf::Vector2i bottomRight(rightX, bottomY);

	// check top side
	int y = topLeft.y;
	for (int x = topLeft.x; x <= topRight.x; x++)
	{
		if (m_collidableTiles[y][x])
		{
			return true;
		}
	}

	// check bottom side
	y = bottomLeft.y;
	for (int x = bottomLeft.x; x <= bottomRight.x; x++)
	{
		if (m_collidableTiles[y][x])
		{
			return true;
		}
	}

	return false;
}

ResourceID Map::checkLevelEntry(const sf::FloatRect& boundingBox)
{
	
	if (boundingBox.contains(sf::Vector2f(50*1, 50*32)))
	{
		return ResourceID::Level_testLevel;
	}
	else 
	{
		return ResourceID::Void;
	}
}