#pragma once

#include "global.h"
#include "Logger.h"

#include "sqlite/sqlite3.h"

struct TextBean;
struct ItemAttributeBean;
struct ItemBean;
struct ItemConvertibleBean;
struct ItemSpellBean;
struct ItemEquipmentBean;
struct ItemEquipmentLightBean;
struct ItemFoodBean;
struct ItemWeaponBean;
struct ItemWeaponSlotBean;
struct LevelitemBean;
struct LevelitemFrameBean;
struct LevelitemLightBean;

typedef std::vector<std::vector<std::string>> ResultSet;

/* interface to the SQLite database */
class DatabaseManager {
public:
	DatabaseManager();
	~DatabaseManager();

	ResultSet query(const std::string& query) const;

	TextBean getTextBean(const std::string& text_id, const std::string& text_type) const;
	ItemAttributeBean getItemAttributeBean(const std::string& item_id) const;
	ItemBean getItemBean(const std::string& item_id) const;
	std::vector<ItemConvertibleBean> getItemConvertibleBeans(const std::string& item_id) const;
	ItemSpellBean getItemSpellBean(const std::string& item_id) const;
	ItemEquipmentBean getItemEquipmentBean(const std::string& item_id) const;
	ItemEquipmentLightBean getItemEquipmentLightBean(const std::string& item_id) const;
	ItemFoodBean getItemFoodBean(const std::string& item_id) const;
	ItemWeaponBean getItemWeaponBean(const std::string& item_id) const;
	ItemWeaponSlotBean getItemWeaponslotBean(const std::string& item_id, int slot_nr) const;
	std::vector<ItemWeaponSlotBean> getItemWeaponSlotBeans(const std::string& item_id) const;
	LevelitemBean getLevelitemBean(const std::string& item_id) const;
	LevelitemFrameBean getLevelitemFrameBean(const std::string& item_id, int frame_nr) const;
	std::vector<LevelitemFrameBean> getLevelitemFrameBeans(const std::string& item_id) const;
	LevelitemLightBean getLevelitemLightBean(const std::string& item_id) const;

	bool itemExists(const std::string& item_id) const;

private:
	const std::string DB_FILENAME = "db/game_data.db";
	void checkError() const;
	void init();

	sqlite3 *m_db = nullptr;
};