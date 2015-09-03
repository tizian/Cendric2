#pragma once

#include <map>

#include "global.h"
#include "ResourceManager.h"
#include "FileIO/CharacterCoreReader.h"
#include "FileIO/CharacterCoreWriter.h"
#include "Item.h"
#include "Weapon.h"

#include "Structs/AttributeBean.h"
#include "Structs/CharacterCoreData.h"

class CharacterCore
{
public:
	CharacterCore();
	CharacterCore(const CharacterCoreData& data);
	~CharacterCore();

	// returns the currently equipped item of type 'type'
	const Item* getEquippedItem(ItemType type);
	// getter for currently equipped weapon. convenience method for getItem of type weapon with a dynamic cast.
	const Weapon* getWeapon();
	// returns the item of id "id"
	const Item& getItem(const std::string& id);
	NPCState getNPCState(NPCID id);
	QuestState getQuestState(QuestID id);
	void setNPCState(NPCID id, NPCState state);
	void setQuestState(QuestID id, QuestState state);
	void setQuickslot(const std::string& item, int nr);
	// loads a new game with a new core and default attributes
	void loadNew();
	// tries to create a new file with this name. if successful, returns true. if it already exists, returns false
	bool createFile(const std::string& fileName) const;
	// uses the character core reader to load a .sav file
	bool load(const std::string& fileName);
	bool quickload();
	// uses the character core writer to save a .sav file. The name is the name chosen by the user.
	bool save(const std::string& fileName, const std::string& name);
	bool quicksave();
	// loads equipped itemvector (reload if not empty)
	void loadEquipmentItems();
	// loads itemvector (reload if not empty)
	void loadItems();
	// base attributes plus the attributes gotten by equipment
	const AttributeBean& getTotalAttributes() const;
	// getter for items
	std::map<std::string, int>* getItems();
	// add gold to the data
	void addGold(int gold);
	// remove gold from the data
	void removeGold(int gold);
	// adds item(s) to the data
	void addItem(const std::string& item, int quantity);
	// removes item(s) to the data
	void removeItem(const std::string& item, int quantity);
	// save the position and map id. also sets "isInLevel" = false
	void setMap(const sf::Vector2f& position, MapID map);
	// save the position and level id  also sets "isInLevel" = true
	void setLevel(const sf::Vector2f& position, LevelID level);
	// getter for core part
	const CharacterCoreData& getData() const;

	static const char* QUICKSAVE_LOCATION;
	static const char* DEBUGSAVE_LOCATION;

private:
	// clears equipped itemvector
	void clearEquippedItems();
	// clears itemvector
	void clearItems();
	// reload attributes based on equipped items (called by load). all attributes coming from food are removed.
	void reloadAttributes();

	// base attributes plus the attributes of all currently equipped items
	AttributeBean m_totalAttributes;

	std::map<std::string, Item> m_items;
	std::map<ItemType, Item*> m_equippedItems;

	CharacterCoreData m_data;

	sf::Clock m_stopwatch; 
};