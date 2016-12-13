#pragma once

#include "World.h"
#include "BackgroundLayer.h"
#include "FileIO/LevelReader.h"
#include "Level/LevelDynamicTile.h"
#include "Level/BossLevel.h"
#include "LevelLoader.h"
#include "SpeedupPullCamera.h"
#include "Structs/AIWalkingQueryRecord.h"

// a sidescroller level
class Level : public World {
public:
	Level();
	~Level();

	// updates all tilemaps and the level camera
	void update(const sf::Time& frameTime) override;

	// loads a .tmx file
	bool load(const std::string& id, WorldScreen* screen) override;
	// loads dynamic tiles and lights. this happens AFTER everything else and is because of our nice RENDERTEXTURE PROBLEM >:(
	void loadForRenderTexture() override;
	// loads enemies and level items for the level. must be called after a screen already has a main char
	void loadAfterMainChar() override;
	void setWorldView(sf::RenderTarget& target, const sf::Vector2f& focus) const override;
	void drawBackgroundLayers(sf::RenderTarget& target, const sf::RenderStates& states, const sf::Vector2f& focus) const;
	// deletes the resources
	void dispose() override;

	// collision with the level, see WorldCollisionQueryRecord for more infos about the argument
	bool collides(WorldCollisionQueryRecord& rec) const override;
	// checks for collisions with tiles that will kill. adds a safety margin to the bb.
	bool collidesWithEvilTiles(const sf::FloatRect& boundingBox) const;
	// checks for collisions with tiles you'd better not touch. adds a safety margin to the bb.
	bool collidesWithAvoidableTiles(const sf::FloatRect& boundingBox) const;
	// checks for collisions with mobs (enemies and level main character)
	bool collidesWithMobs(WorldCollisionQueryRecord& rec, bool isInitialQuery = true) const;
	// checks for collisions with movable tiles (moving platforms, shiftable blocks and unstable blocks)
	bool collidesWithMovableTiles(WorldCollisionQueryRecord& rec) const;

	void collideWithDynamicTiles(Spell* spell, const sf::FloatRect& boundingBox) const;
	void collideWithDynamicTiles(LevelMovableGameObject* mob, const sf::FloatRect& boundingBox) const;

	const std::vector<GameObject*>* getMovableTiles() const;
	const std::vector<GameObject*>* getDynamicTiles() const;
	const LevelData* getWorldData() const;

	// executes the boss ending, either win=true or else if the player loses.
	void executeBossEnding(bool win);

private:
	// data loaded by the level loader
	LevelData m_levelData;
	std::vector<GameObject*>* m_dynamicTiles;
	std::vector<GameObject*>* m_movableTiles;
	BossLevel* m_bossLevel = nullptr;

	// checks for collisions with those specific tiles
	bool collidesWithSpecificTiles(const sf::FloatRect& boundingBox, const std::set<LevelDynamicTileID>& tiles) const;

	SpeedupPullCamera* m_camera;
	const float CAMERA_WINDOW_WIDTH = 200.f;
	const float CAMERA_WINDOW_HEIGHT = 200.f;

	std::set<LevelDynamicTileID> m_avoidableTiles;
	std::set<LevelDynamicTileID> m_evilTiles;
};