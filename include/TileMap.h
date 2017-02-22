#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "Structs/WorldData.h"
#include "AnimatedTile.h"

class TileMap : public sf::Drawable, public sf::Transformable {
public:
	bool load(const WorldData& data, const std::vector<std::vector<int> >& layers);
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(const sf::Time& frameTime);
	void dispose();

	void initFogOfWar(const sf::Vector2i& mapSize);
	void updateFogOfWar(const std::vector<bool>& tilesExplored);

private:
	// there is a border around each tile of size 1, to avoid rounding problems
	const int TILE_BORDER = 1;
	std::vector<sf::VertexArray> m_layers;
	std::map<int, std::vector<AnimatedTile*>> m_animatedTiles;
	sf::Texture* m_tileset = nullptr;
	sf::String m_tilesetPath;
	sf::Vector2i m_size;

	void readAnimatedTile(int tileNumber, int layerNr, int i, int j, const WorldData& data);
};