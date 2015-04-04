#pragma once

#include <vector>

#include "global.h"
#include "ResourceManager.h"

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
	bool load(std::string &filepath, sf::Vector2i tilesize, std::vector<std::vector<int> > layers, int width, int height);
	void dispose();
	sf::Vector2i& getTilesize();
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	std::vector<sf::VertexArray> m_layers;
	sf::Texture* m_tileset;
	sf::String m_tilesetPath;
	sf::Vector2i m_tilesize;
};