#pragma once

#include <map>

#include "global.h"
#include "ResourceManager.h"
#include "CharacterCoreReader.h"
#include "Item.h"
#include "ItemFactory.h"

#include "Enums/ItemID.h"
#include "Enums/LevelEquipmentID.h"
#include "Structs/AttributeBean.h"
#include "Structs/CharacterCoreData.h"

class CharacterCore
{
public:
	CharacterCore();
	~CharacterCore();

	// returns the currently equipped item of type 'type'
	const Item& getEquippedItem(ItemType type);
	// loads a new game with a new core and default attributes
	void loadNew();
	// uses the character core reader to load a .sav file
	bool load(char* fileName);
	// uses the character core writer to save a .sav file
	void save(char* fileName);

	// getter for core part
	CharacterCoreData& getData();

private:
	// loads equipped itemvector
	void loadEquipmentItems();
	// loads itemvector
	void loadItems();
	// clears equipped itemvector
	void clearEquippedItems();
	// clears itemvector
	void clearItems();
	// reload stats based on equipped items (called by load)
	void reloadStats();
	// adds the second bean to the first bean
	void addBean(AttributeBean& firstBean, const AttributeBean& secondBean) const;

	// base attributes plus the attributes of all currently equipped items
	AttributeBean m_totalAttributes;

	std::map<ItemID, Item> m_items;
	std::map<ItemType, Item> m_equippedItems;

	CharacterCoreData m_data;

	sf::Clock m_stopwatch; 
};