#pragma once

#include "global.h"
#include "World/AnimatedGameObject.h"
#include "ResourceManager.h"
#include "Structs/MapDynamicTileData.h"

class Map;
class MapScreen;
class MapMainCharacter;

// A dynamic tile in a map
class MapDynamicTile : public virtual AnimatedGameObject {
public:
	MapDynamicTile(MapScreen* mapScreen);
	virtual ~MapDynamicTile() {};

	// load the dynamic tile, returns whether successful.
	virtual bool init(const MapTileProperties& properties) = 0;
	// loads the resources (sprite, sound)
	virtual void loadResources();
	// loads the dynamic tile with the specified skin nr
	virtual void loadAnimation(int skinNr) = 0;
	// gets called by the main character when it hits the dynamic tile. default does nothing
	virtual void onHit(MapMainCharacter* mob);

	// sets the position offset for a dynamic tile. DON'T use that for collidable dynamic tiles.
	void setPositionOffset(const sf::Vector2f& offset);

	const sf::Vector2f& getPositionOffset() const;
	bool isCollidable() const;
	GameObjectType getConfiguredType() const override;
	virtual MapDynamicTileID getDynamicTileID() const = 0;

protected:
	virtual std::string getSpritePath() const { return ""; }
	virtual std::string getSoundPath() const { return ""; }
	// dynamic tile textures have a border (border width in pixel)
	const int BORDER = 1;
	bool m_isCollidable = false;
	const Map* m_map;
	MapMainCharacter* m_mainChar;

private:
	sf::Vector2f m_positionOffset = sf::Vector2f(0.f, 0.f);
};