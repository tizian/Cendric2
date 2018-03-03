#pragma once

#include "World/World.h"
#include "World/BackgroundLayer.h"
#include "FileIO/LevelReader.h"
#include "Level/LevelDynamicTile.h"
#include "Level/BossLevel.h"
#include "LevelLoader.h"
#include "World/Camera/SpeedupPullCamera.h"
#include "World/Camera/AutoscrollerCamera.h"
#include "Structs/AIWalkingQueryRecord.h"

// a sidescroller level
class Level final : public World {
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
	void loadAfterMainChar(MainCharacter* mainChar) override;
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
	// checks whether there's a collision with the camera (autoscroller)
	bool collidesWithCamera(const sf::Vector2f& nextPos) const;

	void collideWithDynamicTiles(Spell* spell, const sf::FloatRect& boundingBox) const;
	void collideWithDynamicTiles(LevelMovableGameObject* mob, const sf::FloatRect& boundingBox) const;
	bool isInsideWorldRect(const sf::FloatRect& boundingBox) const override;

	void raycast(RaycastQueryRecord& rec) const;

	const std::vector<GameObject*>* getMovableTiles() const;
	const std::vector<GameObject*>* getDynamicTiles() const;
	const LevelData* getWorldData() const override { return &m_levelData; };

	void setBackgroundLayerColor(const sf::Color& color) const;

	// executes the boss ending, either win=true or else if the player loses.
	void executeBossEnding(bool win);

	// handling of locked magic types
	void setMagicLocked(SpellType type);
	bool isMagicLocked(SpellType type);
	const std::set<SpellType>& getLockedMagic() const;

	

private:
	void loadCamera(LevelMainCharacter* mainChar);

private:
	// data loaded by the level loader
	LevelData m_levelData;
	std::vector<GameObject*>* m_dynamicTiles;
	std::vector<GameObject*>* m_movableTiles;
	BossLevel* m_bossLevel = nullptr;

	// checks for collisions with those specific tiles
	bool collidesWithSpecificTiles(const sf::FloatRect& boundingBox, const std::set<LevelDynamicTileID>& tiles) const;

	Camera* m_camera = nullptr;
	static const float CAMERA_WINDOW_WIDTH;
	static const float CAMERA_WINDOW_HEIGHT;

	std::set<LevelDynamicTileID> m_avoidableTiles;
	std::set<LevelDynamicTileID> m_evilTiles;
	std::set<SpellType> m_lockedMagicTypes;

private:
	void raycastWorld(RaycastQueryRecord& rec) const;
	void raycastDynamicTiles(RaycastQueryRecord& rec) const;
	bool isTileCollidable(int x, int y) const;
};