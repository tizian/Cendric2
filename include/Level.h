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

	// checks collision with the collidable grid of that level
	bool collidesX(const sf::FloatRect& boundingBox);
	bool collidesY(const sf::FloatRect& boundingBox);
	// returns the next y position where the bounding box is grounding.
	float getGround(const sf::FloatRect& boundingBox);

	// loads a .dric file
	bool load(ResourceID id);

	// deletes the resources
	void dispose();

	// draws the level. Background layers, tilemap, foreground layers.
	void draw(sf::RenderTarget &target, sf::RenderStates states, const Vector2f& center);

private:
	TileMap m_tileMap;
	FloatRect m_levelRect;
	vector<vector<bool>> m_collidableTiles;
	vector<BackgroundLayer> m_backgroundLayers;
	std::string m_name;
	sf::Vector2f m_startPos;

	ResourceID m_resourceID;
};