#include "stdafx.h"

class Level 
{
public:
	Level();
	~Level();

	// getters
	TileMap getTilemap();
	Vector2f& getStartPos();
	FloatRect& getLevelRect();
	vector<sf::FloatRect>& getCollidableTiles();

	// \brief loads a .dric file
	bool load(ResourceID id);

	// deletes the resources
	void dispose();

	// \brief draws the level. Background layers, tilemap, foreground layers.
	void draw(sf::RenderTarget &target, sf::RenderStates states, const Vector2f& center);

private:
	TileMap m_tileMap;
	FloatRect m_levelRect;
	vector<sf::FloatRect> m_collidableTiles;
	vector<BackgroundLayer> m_backgroundLayers;
	std::string m_name;
	sf::Vector2f m_startPos;

	ResourceID m_resourceID;
};