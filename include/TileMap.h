#pragma once

#include <vector>

#include "global.h"
#include "ResourceManager.h"

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
	bool load(const std::string& filepath, const sf::Vector2i& tilesize, const std::vector<std::vector<int> >& layers, int width, int height);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void dispose();
	
	const sf::Vector2i& getTilesize() const;
	
private:
	// there is a border around each tile of size 1, to avoid rounding problems
	const int TILE_BORDER = 1;
	std::vector<sf::VertexArray> m_layers;
	sf::Texture* m_tileset;
	sf::String m_tilesetPath;
	sf::Vector2i m_tilesize;
};