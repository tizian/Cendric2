#include "stdafx.h"

class Level 
{
public:
	Level();
	~Level();

	// getters
	TileMap getTilemap();

	// \brief loads a .dric file
	bool loadFromFile(char* fileName);

	// \brief draws the level. At the moment, only its tilemap.
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	TileMap m_tileMap;
	vector<bool> m_collidableTiles;
	std::string m_name;
	sf::Vector2i m_startPos;
};