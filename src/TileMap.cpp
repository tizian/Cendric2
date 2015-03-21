#include "stdafx.h"

bool TileMap::load(const std::string &filepath, sf::Vector2i tileSize, vector<vector<int> > layers, int width, int height)
{
	if (!m_tileset.loadFromFile(filepath))
	{
		printf("TileMap: Error at opening file %s \n", filepath.c_str());
		return false;
	}

	m_layers.clear();

	for (int count = 0; count < layers.size(); count++)
	{
		VertexArray layer;

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

				int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
				int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

				sf::Vertex *quad = &layer[(i + j * width) * 4];

				quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
				quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
				quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
				quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

				quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
				quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
				quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
				quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
			}
		}

		m_layers.push_back(layer);
	}

	return true;
}

void TileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &m_tileset;
	for (int i = 0; i < m_layers.size(); i++)
	{
		target.draw(m_layers[i], states);
	}
}