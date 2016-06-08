#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "GUI/LootWindow.h"
#include "Structs/ChestData.h"

class LevelMainCharacter;
class InteractComponent;
class LightComponent;

class ChestTile : public LevelDynamicTile {
public:
	ChestTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	void renderAfterForeground(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void onRightClick() override;
	void onMouseOver() override;
	void loot();
	
	void setChestData(const ChestData& data);

private:
	std::string getSpritePath() const override;
	// the object id in the dynamic chest tiles object layer.
	int m_objectID = -1;
	// strength of the chest lock. It ranges from 0 to 3. Levels 1 to 3 need an unlock spell to unlock, level 0 only a right click :)
	int m_strength = 0;
	// a permanent chest will stay in an "opened" state after it has been looted. It cannot be looted again though.
	bool m_isPermanent = false;
	// lootable items 
	std::map<std::string, int> m_lootableItems;
	int m_lootableGold = 0;
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