#include "stdafx.h"

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
}

bool Level::load(ResourceID id) 
{
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
	return true;
}

void Level::draw(sf::RenderTarget &target, sf::RenderStates states)
{
	for (int i = 0; i < m_backgroundLayers.size(); i++)
	{
		m_backgroundLayers[i].render(target);
	}
	m_tileMap.draw(target, states);
}

FloatRect& Level::getLevelRect()
{
	return m_levelRect;
}

TileMap Level::getTilemap() 
{
	return m_tileMap;
}

Vector2f& Level::getStartPos()
{
	return m_startPos;
}

vector<sf::FloatRect>& Level::getCollidableTiles()
{
	return m_collidableTiles;
}