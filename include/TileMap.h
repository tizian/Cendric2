#include "stdafx.h"

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
	bool load(const std::string &filepath, sf::Vector2i tilesize, vector<vector<int>> layers, int width, int height);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	vector<sf::VertexArray> m_layers;
	sf::Texture m_tileset;
};