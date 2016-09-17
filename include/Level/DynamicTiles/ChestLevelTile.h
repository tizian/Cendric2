#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "GUI/LootWindow.h"
#include "Structs/ChestTileData.h"

class LevelMainCharacter;
class InteractComponent;
class LightComponent;

class ChestLevelTile : public LevelDynamicTile {
public:
	ChestLevelTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	void renderAfterForeground(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void onRightClick() override;
	void onMouseOver() override;
	void loot();
	
	void setChestData(const ChestTileData& data);
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Chest; }

private:
	std::string getSpritePath() const override;
	ChestTileData m_data;
	void setLoot(const std::map<std::string, int>& items, int gold);
	void unlock();

	InteractComponent* m_interactComponent = nullptr;
	// The light will disappear (set invisible) on looted.
	LightComponent* m_lightComponent = nullptr;

	// the chest can only be looted if the main char is in this range
	const float PICKUP_RANGE = 100.f;
	LootWindow* m_lootWindow = nullptr;
	bool m_showLootWindow = false;
};