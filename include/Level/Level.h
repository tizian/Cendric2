#pragma once

#include "World.h"
#include "BackgroundLayer.h"
#include "FileIO/LevelReader.h"
#include "Level/LevelDynamicTile.h"
#include "LevelLoader.h"
#include "SpeedupPullCamera.h"
#include "Structs/LevelExitData.h"
#include "Structs/AIWalkingQueryRecord.h"

// a sidescroller level
class Level : public World {
public:
	Level();
	~Level();

	// updates all tilemaps and the level camera
	void update(const sf::Time& frameTime) override;

	// loads a .tmx file
	bool load(const std::string& id) override;
	// loads dynamic tiles and lights. this happens AFTER everything else and is because of our nice RENDERTEXTURE PROBLEM >:(
	void loadForRenderTexture(Screen* screen) override;
	// loads enemies and level items for the level. must be called after a screen already has a main char
	void loadAfterMainChar(Screen* screen) override;
	void setWorldView(sf::RenderTarget& target, const sf::Vector2f& focus) const override;
	void drawBackgroundLayers(sf::RenderTarget& target, const sf::RenderStates& states, const sf::Vector2f& focus) const;
	// deletes the resources
	void dispose() override;

	// checks if the main char has reached a level exit. If no, it returns nullptr, the map id and spawn point for the map.
	LevelExitData* checkLevelExit(const sf::FloatRect& boundingBox) const;
	// collision with the level, see WorldCollisionQueryRecord for more infos about the argument
	bool collides(WorldCollisionQueryRecord& rec) const override;
	// checks for collisions with those specific dynamic tiles
	bool collidesWithDynamicTiles(const sf::FloatRect& boundingBox, const std::set<LevelDynamicTileID>& tiles) const;
	// checks for collisions with mobs (enemies and level main character)
	bool collidesWithMobs(WorldCollisionQueryRecord& rec, bool isInitialQuery = true) const;
	// checks for collisions with movable tiles (moving platforms, shiftable blocks and unstable blocks)
	bool collidesWithMovableTiles(WorldCollisionQueryRecord& rec) const;

	void collideWithDynamicTiles(Spell* spell, const sf::FloatRect& boundingBox) const;
	void collideWithDynamicTiles(LevelMovableGameObject* mob, const sf::FloatRect& boundingBox) const;

private:
	// data loaded by the level loader
	LevelData m_levelData;
	std::vector<GameObject*>* m_dynamicTiles;
	std::vector<GameObject*>* m_movableTiles;

	SpeedupPullCamera* m_camera;
	const float CAMERA_WINDOW_WIDTH = 200.f;
	const float CAMERA_WINDOW_HEIGHT = 200.f;
};