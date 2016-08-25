#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "GUI/LootWindow.h"
#include "Structs/ResourceTileData.h"

class LevelMainCharacter;
class InteractComponent;

class ResourceTile : public LevelDynamicTile {
public:
	ResourceTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	void init() override;
	void loadAnimation(int skinNr) override;
	void onRightClick() override;
	void onHit(Spell* spell) override {};
	void loot();
	
	void setResourceTileData(const ResourceTileData& data);
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Resource; }

private:
	std::string getSpritePath() const override;
	// the object id in the dynamic chest tiles object layer.
	int m_objectID = -1;
	// loot that can be gotten from that tile
	std::map<std::string, int> m_lootableItems;
	int m_lootableGold = 0;

	InteractComponent* m_interactComponent = nullptr;
	
	// the resource can only be gotten if the main char is in this range
	const float PICKUP_RANGE = 100.f;
};