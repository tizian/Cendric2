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
	m_backgroundTileMap.dispose();
	m_foregroundTileMap.dispose();
}

bool Map::load(const std::string& id)
{
	MapReader reader;
	if (!reader.readMap(id.c_str(), m_mapData))
	{
		return false;
	}
	
	// load map
	m_backgroundTileMap.load(m_mapData.tileSetPath, m_mapData.tileSize, m_mapData.backgroundLayers, m_mapData.mapSize.x, m_mapData.mapSize.y);
	m_foregroundTileMap.load(m_mapData.tileSetPath, m_mapData.tileSize, m_mapData.foregroundLayers, m_mapData.mapSize.x, m_mapData.mapSize.y);
	m_id = id;
	return true;
}

void Map::loadAfterMainChar(Screen* screen)
{
	MapLoader loader;
	loader.loadNpcs(m_mapData, screen);
}

void Map::draw(sf::RenderTarget &target, const sf::RenderStates states, const sf::Vector2f& center, const TileMap& map) const
{
	sf::View view;
	view.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

	float camCenterX = std::max(WINDOW_WIDTH / 2.f, std::min(m_mapData.mapRect.width - WINDOW_WIDTH / 2.f, center.x));
	float camCenterY = std::max((WINDOW_HEIGHT) / 2.f, std::min(m_mapData.mapRect.height - (WINDOW_HEIGHT) / 2.f, center.y));
	view.setCenter(camCenterX, camCenterY);
	target.setView(view);
	map.draw(target, states);
}

void Map::drawBackground(sf::RenderTarget &target, const sf::RenderStates states, const sf::Vector2f& center) const
{
	draw(target, states, center, m_backgroundTileMap);
}

void Map::drawForeground(sf::RenderTarget &target, const sf::RenderStates states, const sf::Vector2f& center) const
{
	draw(target, states, center, m_foregroundTileMap);
}

const sf::FloatRect& Map::getMapRect() const
{
	return m_mapData.mapRect;
}

const TileMap& Map::getBackgroundTilemap() const 
{
	return m_backgroundTileMap;
}

const TileMap& Map::getForegroundTilemap() const
{
	return m_foregroundTileMap;
}

bool Map::collidesX(const sf::FloatRect& boundingBox) const
{
	// check for collision with level rect
	if (boundingBox.left < m_mapData.mapRect.left || boundingBox.left + boundingBox.width > m_mapData.mapRect.left + m_mapData.mapRect.width)
	{
		return true;
	}

	float tileWidth = static_cast<float>(m_backgroundTileMap.getTilesize().x);
	float tileHeight = static_cast<float>(m_backgroundTileMap.getTilesize().y);

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
		if (m_mapData.collidableTileRects[y][x])
		{
			return true;
		}
	}

	// check right side
	x = topRight.x;
	for (int y = topRight.y; y <= bottomRight.y; y++)
	{
		if (m_mapData.collidableTileRects[y][x])
		{
			return true;
		}
	}

	return false;
}

bool Map::collidesY(const sf::FloatRect& boundingBox) const
{
	// check for collision with level rect
	if (boundingBox.top < m_mapData.mapRect.top || boundingBox.top + boundingBox.height > m_mapData.mapRect.top + m_mapData.mapRect.height)
	{
		return true;
	}

	float tileWidth = static_cast<float>(m_backgroundTileMap.getTilesize().x);
	float tileHeight = static_cast<float>(m_backgroundTileMap.getTilesize().y);

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
		if (m_mapData.collidableTileRects[y][x])
		{
			return true;
		}
	}

	// check bottom side
	y = bottomLeft.y;
	for (int x = bottomLeft.x; x <= bottomRight.x; x++)
	{
		if (m_mapData.collidableTileRects[y][x])
		{
			return true;
		}
	}

	return false;
}

MapExitBean* Map::checkLevelEntry(const sf::FloatRect& boundingBox) const
{
	for (auto it : m_mapData.mapExits)
	{
		if (boundingBox.intersects(it.mapExitRect))
		{
			MapExitBean* exit = new MapExitBean();
			exit->levelID = it.levelID;
			exit->levelSpawnPoint = it.levelSpawnPoint;
			return exit;
		}
	}

	return nullptr;
}

const std::string& Map::getID() const
{
	return m_id;
}