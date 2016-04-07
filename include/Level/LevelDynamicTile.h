#pragma once

#include "global.h"
#include "AnimatedGameObject.h"
#include "ResourceManager.h"
#include "Enums/LevelDynamicTileID.h"

class Spell;
class LevelMovableGameObject;
class Level;
class LevelMainCharacter;
class LevelScreen;

// A dynamic tile in a level
class LevelDynamicTile : public virtual AnimatedGameObject {
public:
	LevelDynamicTile(LevelScreen* levelScreen);
	virtual ~LevelDynamicTile() {};

	// loads sprite offset & bounding box.
	virtual void init() = 0;
	// loads the dynamic tile with the specified skin nr
	virtual void loadAnimation(int skinNr) = 0;
	// gets called by a spell when it hits the dynamic tile
	virtual void onHit(Spell* spell) = 0;
	// gets called by a mob when it hits the dynamic tile. default does nothing
	virtual void onHit(LevelMovableGameObject* mob);
	// the render method of level dynamic tiles gets called twice per game loop
	// this is to seperate the water tiles which are rendered after the game objects
	// from the other tiles.
	virtual void render(sf::RenderTarget& target) override;
	virtual void update(const sf::Time& frameTime) override;

	// sets the position offset for a dynamic tile. DON'T use that for collidable dynamic tiles.
	void setPositionOffset(const sf::Vector2f& offset);
	void setDynamicTileID(LevelDynamicTileID id);

	const sf::Vector2f& getPositionOffset() const;
	LevelDynamicTileID getDynamicTileID() const;
	bool getIsCollidable() const;
	bool getIsStrictlyCollidable() const;
	GameObjectType getConfiguredType() const override;

protected:
	// dynamic tile textures have a border (border width in pixel)
	const int BORDER = 1;
	const Level* m_level;
	LevelMainCharacter* m_mainChar;
	LevelDynamicTileID m_dynamicTileID;
	bool m_isCollidable = false;
	bool m_isStrictlyCollidable = false;
	bool m_isRenderAfterObjects = false;
	bool m_isFirstRenderIteration = false;

private:
	sf::Vector2f m_positionOffset = sf::Vector2f(0.f, 0.f);
};