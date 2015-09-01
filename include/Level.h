#pragma once

#include <vector>
#include <math.h>

#include "global.h"
#include "TileMap.h"
#include "BackgroundLayer.h"
#include "FileIO/LevelReader.h"
#include "ResourceManager.h"
#include "DynamicTile.h"
#include "Logger.h"
#include "LevelLoader.h"
#include "SpeedupPullCamera.h"

#include "Enums/LevelID.h"

class Level 
{
public:
	Level();
	~Level();

	// loads a .dric file
	bool load(LevelID id);
	// loads only dynamic tiles. this happens AFTER everything other and is because of our nice RENDERTEXTURE PROBLEM >:(
	void loadDynamicTiles(Screen* screen);
	// loads enemies and level items for the level. must be called after a screen already has a main char
	void loadAfterMainChar(Screen* screen);
	// draws the level. Background layers, background tilemap, dynamic tiles
	void drawBackground(sf::RenderTarget &target, const sf::RenderStates& states, const sf::Vector2f& center) const;
	// draws the level. Foreground tilemap
	void drawForeground(sf::RenderTarget &target, const sf::RenderStates& states, const sf::Vector2f& center) const;
	// deletes the resources
	void dispose();

	// checks if the main char has reached a level exit. If no, it returns nullptr, the map id and spawn point for the map.
	LevelExitBean* checkLevelExit(const sf::FloatRect& boundingBox) const;
	// formula for the jump height is vel_y_max^2 / (2*gravity acc)
	bool collidesAfterJump(const sf::FloatRect& boundingBox, float jumpHeight, bool right) const;
	// calculates if the object would fall deeper than it can jump if it did one more step in the given direction.
	bool fallsDeep(const sf::FloatRect& boundingBox, float jumpHeight, bool right, float stepSize) const;
	// checks collision with the collidable grid of that level and also the collidable dynamic tiles
	// if the calling object itself wants to be excluded, it can give itself as an argument here.
	bool collidesX(const sf::FloatRect& boundingBox, const DynamicTile* exclude) const;
	bool collidesY(const sf::FloatRect& boundingBox, const DynamicTile* exclude) const;

	bool collidesX(const sf::FloatRect& boundingBox) const;
	bool collidesY(const sf::FloatRect& boundingBox) const;

	bool collidesLevelBottom(const sf::FloatRect& boundingBox) const;
	bool collidesLevelCeiling(const sf::FloatRect& boundingBox) const;
	void collideWithDynamicTiles(Spell* spell, const sf::FloatRect* boundingBox) const;
	void collideWithDynamicTiles(LevelMovableGameObject* mob, const sf::FloatRect* boundingBox) const;

	const TileMap& getTilemap() const;
	const sf::FloatRect& getLevelRect() const;
	LevelID getID() const;
	// returns the next y position where the bounding box is grounding.
	float getGround(const sf::FloatRect& boundingBox) const;
	// returns the next y position where the bounding box is on the ceiling.
	float getCeiling(const sf::FloatRect& boundingBox) const;

private:
	TileMap m_backgroundTileMap;
	TileMap m_foregroundTileMap;
	std::vector<GameObject*>* m_dynamicTiles;
	LevelID m_id;
	// data loaded by the level loader
	LevelData m_levelData;

	SpeedupPullCamera* m_camera;
	const float CAMERA_WINDOW_WIDTH = 200.f;
	const float CAMERA_WINDOW_HEIGHT = 200.f;
	float tileHeight;
	float tileWidth;
};