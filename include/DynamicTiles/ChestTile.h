#pragma once

#include "global.h"
#include "DynamicTile.h"
#include "GUI/LootWindow.h"

class LevelMainCharacter;

class ChestTile : public DynamicTile
{
public:
	ChestTile(LevelMainCharacter* mainChar, Level* level);
	void renderAfterForeground(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;
	void init() override;
	void load(int skinNr) override;
	void onHit(Spell* spell) override;
	void onRightClick() override;
	void onMouseOver() override;
	// the object id in the dynamic chest tiles object layer.
	void setObjectID(int id);

	void setLoot(const std::map<std::string, int> &items, int gold);

private:
	LevelMainCharacter* m_mainChar;
	int m_objectID = -1;
	// lootable items 
	std::map<std::string, int> m_lootableItems;
	int m_lootableGold = 0;
	LootWindow* m_lootWindow = nullptr;
	bool m_showLootWindow = false;

	// the chest can only be looted if the main char is in this range
	const float PICKUP_RANGE = 100.f;
};