#pragma once

#include "global.h"
#include "AnimatedGameObject.h"
#include "ResourceManager.h"
#include "Enums/LevelDynamicTileID.h"

class Spell;
class LevelMovableGameObject;
class Level;

// A dynamic tile in a level
class LevelDynamicTile : public virtual AnimatedGameObject {
public:
	LevelDynamicTile(Level* level);
	virtual ~LevelDynamicTile() {};

	// loads sprite offset & bounding box.
	virtual void init() = 0;
	// loads the dynamic tile with the specified skin nr
	virtual void loadAnimation(int skinNr) = 0;
	// gets called by a spell when it hits the dynamic tile
	virtual void onHit(Spell* spell) = 0;
	// gets called by a mob when it hits the dynamic tile. default does nothing
	virtual void onHit(LevelMovableGameObject* mob);

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
	Level* m_level;
	LevelDynamicTileID m_dynamicTileID;
	bool m_isCollidable = false;
	bool m_isStrictlyCollidable = false;

private:
	sf::Vector2f m_positionOffset = sf::Vector2f(0.f, 0.f);
};