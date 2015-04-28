#include "Level.h"
#include "Screen.h"

using namespace std;

Level::Level()
{
}

Level::~Level()
{
	dispose();
}

void Level::dispose()
{
	m_tileMap.dispose();
	for (int i = 0; i < m_backgroundLayers.size(); i++)
	{
		m_backgroundLayers[i].dispose();
	}
	g_resourceManager->deleteLevelResources();
}

bool Level::load(ResourceID id, Screen* screen) 
{
	g_resourceManager->loadLevelResources();
	LevelReader reader;
	LevelData data;
	if (!reader.readLevel(g_resourceManager->getFilename(id), data))
	{
		return false;
	}

	// load level
	m_startPos = data.startPos;
	m_name = data.name;
	m_tileMap.load(data.tileSetPath, data.tileSize, data.layers, data.mapSize.x, data.mapSize.y);
	m_collidableTiles = data.collidableTileRects;
	m_levelRect = data.levelRect;
	m_backgroundLayers = data.backgroundLayers;
	loadDynamicTiles(data, screen);
	return true;
}

void Level::draw(sf::RenderTarget &target, sf::RenderStates states, const sf::Vector2f& center)
{

	sf::View view;
	view.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, static_cast<float>(WINDOW_HEIGHT) / (WINDOW_HEIGHT + BOTTOM_BORDER)));

	// parallax background layers
	for (int i = 0; i < m_backgroundLayers.size(); i++)
	{
		// handle case for layer at infinity
		if (m_backgroundLayers[i].getDistance() == -1.0f) 
		{
			view.setCenter(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
			target.setView(view);
		}
		else
		{
			float d = m_backgroundLayers[i].getDistance();
			float ominoeseOffset = (WINDOW_WIDTH / 2) - (1 / d) * (WINDOW_WIDTH / 2);
			float camCenterX = (std::max(WINDOW_WIDTH / 2.f, std::min(m_levelRect.width - WINDOW_WIDTH / 2.f, center.x)) / d) + ominoeseOffset;
			float camCenterY = WINDOW_HEIGHT / 2.f;
			view.setCenter(camCenterX, camCenterY);
			target.setView(view);
		}
		m_backgroundLayers[i].render(target, states);
	}

	// tilemap
	float camCenterX = std::max(WINDOW_WIDTH / 2.f, std::min(m_levelRect.width - WINDOW_WIDTH / 2.f, center.x));
	float camCenterY = WINDOW_HEIGHT / 2.f;
	view.setCenter(camCenterX, camCenterY);
	target.setView(view);
	m_tileMap.draw(target, states);

	// dynamic tiles
	for (std::vector<GameObject*>::iterator it = m_dynamicTiles->begin(); it != m_dynamicTiles->end(); it++)
	{
		(*it)->render(target);
	}

	// foreground layers?
}

sf::FloatRect& Level::getLevelRect()
{
	return m_levelRect;
}

TileMap Level::getTilemap() 
{
	return m_tileMap;
}

sf::Vector2f& Level::getStartPos()
{
	return m_startPos;
}

bool Level::collidesX(const sf::FloatRect& boundingBox)
{
	// check for collision with level rect
	if (boundingBox.left < m_levelRect.left || boundingBox.left + boundingBox.width > m_levelRect.left + m_levelRect.width) 
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

	// check collidable dynamic tiles
	for (std::vector<GameObject*>::iterator it = m_dynamicTiles->begin(); it != m_dynamicTiles->end(); ++it)
	{
		DynamicTile* tile = dynamic_cast<DynamicTile*>((*it));
		if (tile != nullptr && tile->getIsCollidable() && tile->getBoundingBox()->intersects(boundingBox))
		{
			return true;
		}
	}

	return false;
}

bool Level::collidesY(const sf::FloatRect& boundingBox)
{
	// check for collision with level rect
	if (boundingBox.top < m_levelRect.top || boundingBox.top + boundingBox.height > m_levelRect.top + m_levelRect.height)
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

	// check collidable dynamic tiles
	for (std::vector<GameObject*>::iterator it = m_dynamicTiles->begin(); it != m_dynamicTiles->end(); ++it)
	{
		DynamicTile* tile = dynamic_cast<DynamicTile*>((*it));
		if (tile != nullptr && tile->getIsCollidable() && tile->getBoundingBox()->intersects(boundingBox))
		{
			return true;
		}
	}

	return false;
}

float Level::getGround(const sf::FloatRect& boundingBox)
{
	// check if ground is level ground
	if (boundingBox.top + boundingBox.height > m_levelRect.top + m_levelRect.height) 
	{
		return (m_levelRect.top + m_levelRect.height) - boundingBox.height; 
	}

	// then, a collidable tile in the grid must be the ground
	float tileHeight = static_cast<float>(m_tileMap.getTilesize().y);
	int y = static_cast<int>(floor((boundingBox.top + boundingBox.height)) / tileHeight);

	return (y * tileHeight) - boundingBox.height;
}

void Level::loadDynamicTiles(LevelData& data, Screen* screen)
{
	for (std::vector<std::pair<DynamicTileID, sf::Vector2f>>::iterator it = data.dynamicTileRects.begin(); it != data.dynamicTileRects.end(); ++it)
	{
		DynamicTile* tile;
		switch (it->first)
		{
		case DynamicTileID::Water:
			tile = new WaterTile();
			break;
		case DynamicTileID::Ice:
			tile = new IceTile();
			break;
		case DynamicTileID::Crumbly_block:
			tile = new CrumblyBlockTile();
			break;
		default:
			// unexpected error
			g_logger->logError("Level", "Dynamic tile was not loaded, unknown id.");
			return;
		}

		tile->setTileSize(data.tileSize);
		tile->load();
		tile->setPosition(it->second);
		screen->addObject(GameObjectType::_DynamicTile, tile);
	}
	m_dynamicTiles = screen->getObjects(GameObjectType::_DynamicTile);
}

void Level::collideWithDynamicTiles(Spell* spell, const sf::FloatRect nextBoundingBoxX, const sf::FloatRect nextBoundingBoxY)
{
	for (std::vector<GameObject*>::iterator it = m_dynamicTiles->begin(); it != m_dynamicTiles->end(); ++it)
	{
		DynamicTile* tile = dynamic_cast<DynamicTile*>((*it));
		if (tile != nullptr && tile->getBoundingBox()->intersects(nextBoundingBoxX) || tile->getBoundingBox()->intersects(nextBoundingBoxY))
		{
			tile->onHit(spell);
		}
	}
}
