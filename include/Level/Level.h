#pragma once

#include <vector>
#include <math.h>

#include "global.h"
#include "TileMap.h"
#include "BackgroundLayer.h"
#include "FileIO/LevelReader.h"
#include "ResourceManager.h"
#include "Level/LevelDynamicTile.h"
#include "Logger.h"
#include "LevelLoader.h"
#include "SpeedupPullCamera.h"
#include "Structs/LevelExitData.h"

class Level {
public:
	Level();
	~Level();

	// loads a .dric file
	bool load(const std::string& id);
	// loads dynamic tiles and lights. this happens AFTER everything else and is because of our nice RENDERTEXTURE PROBLEM >:(
	void loadForRenderTexture(Screen* screen);
	// loads enemies and level items for the level. must be called after a screen already has a main char
	void loadAfterMainChar(Screen* screen);
	// draws the level. Background layers, background tilemap
	void drawBackground(sf::RenderTarget &target, const sf::RenderStates& states, const sf::Vector2f& center) const;
	// draws the level. Lighted foreground tilemap
	void drawLightedForeground(sf::RenderTarget &target, const sf::RenderStates& states, const sf::Vector2f& center) const;
	// draws the level. Foreground tilemap
	void drawForeground(sf::RenderTarget &target, const sf::RenderStates& states, const sf::Vector2f& center) const;
	// updates all tilemaps and the level camera
	void update(const sf::Time& frameTime);
	// deletes the resources
	void dispose();

	// checks if the main char has reached a level exit. If no, it returns nullptr, the map id and spawn point for the map.
	LevelExitData* checkLevelExit(const sf::FloatRect& boundingBox) const;
	// formula for the jump height is vel_y_max^2 / (2*gravity acc)
	bool collidesAfterJump(const sf::FloatRect& boundingBox, float jumpHeight, bool right, bool ignoreDynamicTiles = false) const;
	// calculates if the object would fall deeper than it can jump if it did one more step in the given direction.
	bool fallsDeep(const sf::FloatRect& boundingBox, float jumpHeight, bool right, float stepSize, bool ignoreDynamicTiles = false) const;
	// checks collision with the collidable grid of that level and also the collidable dynamic tiles
	// if the calling object itself wants to be excluded, it can give itself as an argument here.
	// the last argument will ignore collidable dynamic tiles in the check, but no strictly collidable dynamic tiles.
	bool collides(const sf::FloatRect& boundingBox, const GameObject* exclude, bool ignoreDynamicTiles = false) const;
	bool collides(const sf::FloatRect& boundingBox, bool ignoreDynamicTiles = false) const;
	// checks for collisions with mobs (enemies and level main character)
	bool collidesWithMobs(const sf::FloatRect& boundingBox) const;

	bool collidesLevelBottom(const sf::FloatRect& boundingBox) const;
	bool collidesLevelCeiling(const sf::FloatRect& boundingBox) const;
	void collideWithDynamicTiles(Spell* spell, const sf::FloatRect* boundingBox) const;
	void collideWithDynamicTiles(LevelMovableGameObject* mob, const sf::FloatRect* boundingBox) const;

	const TileMap& getTilemap() const;
	const sf::FloatRect& getLevelRect() const;
	const std::string& getID() const;
	const std::string& getMusicPath() const;
	// returns the next y position where the bounding box is grounding.
	float getGround(const sf::FloatRect& boundingBox) const;
	// returns the next y position where the bounding box is on the ceiling.
	float getCeiling(const sf::FloatRect& boundingBox) const;
	// returns the global dimming factor (0 to 1)
	float getDimming() const;

private:
	TileMap m_backgroundTileMap;
	TileMap m_lightedForegroundTileMap;
	TileMap m_foregroundTileMap;
	std::vector<GameObject*>* m_dynamicTiles;
	Screen* m_screen;
	// data loaded by the level loader
	LevelData m_levelData;

	SpeedupPullCamera* m_camera;
	const float CAMERA_WINDOW_WIDTH = 200.f;
	const float CAMERA_WINDOW_HEIGHT = 200.f;
	float tileHeight;
	float tileWidth;
};