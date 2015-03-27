#include "stdafx.h"

class Level 
{
public:
	Level();
	~Level();

	// getters
	TileMap getTilemap();
	Vector2f& getStartPos();
	vector<sf::FloatRect>& getCollidableTiles();

	// \brief loads a .dric file
	bool loadFromFile(char* fileName);

	// \brief draws the level. At the moment, only its tilemap.
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	TileMap m_tileMap;
	vector<sf::FloatRect> m_collidableTiles;
	std::string m_name;
	sf::Vector2f m_startPos;
};