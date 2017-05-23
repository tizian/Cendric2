#pragma once

#include "global.h"
#include "Structs/LightData.h"

class InteractComponent;

// a chest/chest like object that can be looted on the map/level
class ChestTile {
public:
	ChestTile() {}
	virtual ~ChestTile() {}

protected:
	bool init(const std::map<std::string, std::string>& properties);

protected:
	InteractComponent* m_interactComponent = nullptr;

protected:
	int m_chestStrength = 0;
	bool m_isStoredItems = false;
	std::string m_keyItemID;
	std::map<std::string, int> m_lootableItems;
	int m_lootableGold = 0;
	bool m_isPermanent = false;
	bool m_isOpen = false;
	std::string m_tooltipText;
	LightData m_lightData;
	std::string m_luapath;
};