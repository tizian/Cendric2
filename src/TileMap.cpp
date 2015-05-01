#include "TileMap.h"

using namespace std;

bool TileMap::load(const string &filepath, const sf::Vector2i& tileSize, const vector<vector<int> >& layers, int width, int height)
{
	m_tilesetPath = filepath;
	m_tileset = g_resourceManager->getTexture(filepath);
	m_tilesize = sf::Vector2i(tileSize.x, tileSize.y);
	float tileWidth = static_cast<float>(tileSize.x);
	float tileHeight = static_cast<float>(tileSize.y);

	m_layers.clear();

	for (int count = 0; count < layers.size(); count++)
	{
		sf::VertexArray layer;

		layer.setPrimitiveType(sf::Quads);
		layer.resize(width * height * 4);

		for (int i = 0; i < width; ++i)
		{
			for (int j = 0; j < height; ++j)
			{
				int tileNumber = layers[count][i + j * width];

				if (tileNumber == 0)
				{
					// there is no tile
					continue;
				}

				// we start with tiles at 1 in the .dric format, but need 0 here
				tileNumber--;

				int tu = tileNumber % (m_tileset->getSize().x / tileSize.x);
				int tv = tileNumber / (m_tileset->getSize().x / tileSize.x);

				sf::Vertex *quad = &layer[(i + j * width) * 4];

				quad[0].position = sf::Vector2f(i * tileWidth, j * tileHeight);
				quad[1].position = sf::Vector2f((i + 1) * tileWidth, j * tileHeight);
				quad[2].position = sf::Vector2f((i + 1) * tileWidth, (j + 1) * tileHeight);
				quad[3].position = sf::Vector2f(i * tileWidth, (j + 1) * tileHeight);

				quad[0].texCoords = sf::Vector2f(tu * (tileWidth + 2 * TILE_BORDER) + TILE_BORDER, tv * (tileHeight + 2 * TILE_BORDER) + TILE_BORDER);
				quad[1].texCoords = sf::Vector2f((tu + 1) * (tileWidth + 2 * TILE_BORDER) - TILE_BORDER, tv * (tileHeight + 2 * TILE_BORDER) + TILE_BORDER);
				quad[2].texCoords = sf::Vector2f((tu + 1) * (tileWidth + 2 * TILE_BORDER) - TILE_BORDER, (tv + 1) * (tileHeight + 2 * TILE_BORDER) - TILE_BORDER);
				quad[3].texCoords = sf::Vector2f(tu * (tileWidth + 2 * TILE_BORDER) + TILE_BORDER, (tv + 1) * (tileHeight + 2 * TILE_BORDER) - TILE_BORDER);
			}
		}

		m_layers.push_back(layer);
	}

	return true;
}

void TileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = m_tileset;
	for (int i = 0; i < m_layers.size(); i++)
	{
		target.draw(m_layers[i], states);
	}
}

void TileMap::dispose()
{
	g_resourceManager->deleteResource(m_tilesetPath);
}

const sf::Vector2i& TileMap::getTilesize() const
{
	return m_tilesize;
}