#pragma once

#include "global.h"
#include "Logger.h"

#include "sqlite/sqlite3.h"

struct ItemAttributeBean;
struct ItemBean;
struct ItemSpellBean;
struct ItemEquipmentBean;
struct ItemEquipmentParticleBean;
struct ItemEquipmentLightBean;
struct ItemFoodBean;
struct ItemWeaponBean;
struct ItemDocumentQuestBean;
struct LevelitemBean;
struct LevelitemLightBean;
struct SpawnBean;
struct ItemWeaponSlotBean;
struct LevelitemFrameBean;
struct DatabaseBean;
class Item;

typedef std::vector<std::vector<std::string>> ResultSet;

/* interface to the SQLite database */
class DatabaseManager final {
	friend class Item;
public:
	DatabaseManager();
	~DatabaseManager();

	ResultSet query(const std::string& query) const;
	bool itemExists(const std::string& item_id) const;
	SpawnBean* getSpawnBean(const std::string& spawn_id) const;

	std::vector<std::string> getAllItemIds() const;

private:
	ItemAttributeBean* getItemAttributeBean(const std::string& item_id) const;
	ItemBean* getItemBean(const std::string& item_id) const;
	std::vector<DatabaseBean*> getItemConvertibleBeans(const std::string& item_id) const;
	ItemSpellBean* getItemSpellBean(const std::string& item_id) const;
	ItemEquipmentBean* getItemEquipmentBean(const std::string& item_id) const;
	ItemEquipmentLightBean* getItemEquipmentLightBean(const std::string& item_id) const;
	ItemEquipmentParticleBean* getItemEquipmentParticleBean(const std::string& item_id) const;
	ItemFoodBean* getItemFoodBean(const std::string& item_id) const;
	ItemWeaponBean* getItemWeaponBean(const std::string& item_id) const;
	ItemWeaponSlotBean* getItemWeaponslotBean(const std::string& item_id, int slot_nr) const;
	std::vector<DatabaseBean*> getItemWeaponSlotBeans(const std::string& item_id) const;
	std::vector<DatabaseBean*> getItemDocumentPageBeans(const std::string& item_id) const;
	std::vector<DatabaseBean*> getItemDocumentQuestBeans(const std::string& item_id) const;
	LevelitemBean* getLevelitemBean(const std::string& item_id) const;
	LevelitemFrameBean* getLevelitemFrameBean(const std::string& item_id, int frame_nr) const;
	std::vector<DatabaseBean*> getLevelitemFrameBeans(const std::string& item_id) const;
	LevelitemLightBean* getLevelitemLightBean(const std::string& item_id) const;

private:
	const std::string DB_FILENAME = "db/game_data.db";
	void checkError() const;
	void init();

	sqlite3* m_db = nullptr;
};