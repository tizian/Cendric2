#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "GUI/LootWindow.h"

class LevelMainCharacter;
class InteractComponent;

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
	// the object id in the dynamic chest tiles object layer.
	void setObjectID(int id);
	// loot for the chest
	void setLoot(const std::map<std::string, int>& items, int gold);
	// strength of the chest lock. It ranges from 0 to 3. Levels 1 to 3 need an unlock spell to unlock, level 0 only a right click :)
	void setStrength(int strength);

private:
	std::string getSpritePath() const override;
	int m_objectID = -1;
	int m_strength = 0;
	// lootable items 
	std::map<std::string, int> m_lootableItems;
	int m_lootableGold = 0;
	LootWindow* m_lootWindow = nullptr;
	bool m_showLootWindow = false;
	InteractComponent* m_interactComponent;

	// the chest can only be looted if the main char is in this range
	const float PICKUP_RANGE = 100.f;
};