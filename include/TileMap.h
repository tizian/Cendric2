#pragma once

#include <SFML/Graphics.hpp>

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
	bool load(const std::string &filepath, sf::Vector2i tilesize, const int *tiles, int width, int height);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
};