#pragma once

#include <map>

#include "global.h"
#include "ResourceManager.h"

#include "Enums/ItemID.h"
#include "Enums/LevelEquipmentID.h"
#include "Structs/AttributeBean.h"

class CharacterCore
{
public:
	CharacterCore();
	~CharacterCore();

	// uses the character core reader to load a .sav file
	void load(char* fileName);
	// uses the character core writer to save a .sav file
	void save(char* fileName) const;

private:
	// equip item and change stats accordingly
	void equip();
	// deequip item and change stats accordingly
	void deequip();
	// reload stats based on equipped item vector (called by equip + deequip + load?)
	void reloadStats();

	// base attributes (can be changed with items that give durable stats)
	AttributeBean m_baseAttributes;
	// base attributes plus the attributes of all currently equipped items
	AttributeBean m_totalAttributes;

	int m_currentHealthPoints;
	int m_currentManaPoints;

	int m_gold;
	
	// current equipment
	LevelEquipmentID m_equipped_head;
	LevelEquipmentID m_equipped_body;
	LevelEquipmentID m_equipped_weapon;
	LevelEquipmentID m_equipped_ring_1;
	LevelEquipmentID m_equipped_ring_2;
	LevelEquipmentID m_equipped_neck;
	LevelEquipmentID m_equipped_back;

	// item vector, the id and the quantity.
	std::map<ItemID, int> m_items;

	// current map and position
	ResourceID m_currentMap;
	sf::Vector2f m_currentMapPos;
	
	// the time played with this savegame
	sf::Time m_timePlayed;

	// TODO: quest progress
};