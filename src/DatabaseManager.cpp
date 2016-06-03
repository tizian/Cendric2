#include "DatabaseManager.h"

#include "Beans/ItemAttributeBean.h"
#include "Beans/ItemBean.h"
#include "Beans/ItemEquipmentBean.h"
#include "Beans/ItemConvertibleBean.h"
#include "Beans/ItemEquipmentLightBean.h"
#include "Beans/ItemFoodBean.h"
#include "Beans/ItemWeaponBean.h"
#include "Beans/ItemWeaponSlotBean.h"
#include "Beans/LevelitemBean.h"
#include "Beans/LevelitemFrameBean.h"
#include "Beans/LevelitemLightBean.h"
#include "Beans/TextBean.h"

DatabaseManager *g_databaseManager;

DatabaseManager::DatabaseManager() {
	init();
}

DatabaseManager::~DatabaseManager() {
	if (m_db != nullptr)
		sqlite3_close(m_db);
}

void DatabaseManager::init() {
	int rc = sqlite3_open(getPath(DB_FILENAME).c_str(), &m_db);

	if (rc) {
		g_logger->logError("DatabaseManager", "Can't open database:" + std::string(sqlite3_errmsg(m_db)));
		return;
	}

	g_logger->logInfo("DatabaseManager", "Opened database successfully");
}

void DatabaseManager::checkError() const {
	std::string error = sqlite3_errmsg(m_db);
	if (error.compare("not an error") != 0) {
		g_logger->logError("DatabaseManager", error);
	}
}

ResultSet DatabaseManager::query(const std::string& query_) const {
	sqlite3_stmt *statement;
	ResultSet results;

	if (sqlite3_prepare_v2(m_db, query_.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		int result = 0;
		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				std::vector<std::string> values;
				for (int col = 0; col < cols; col++) {
					values.push_back((char*)sqlite3_column_text(statement, col));
				}
				results.push_back(values);
			}
			else {
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return results;
}

TextBean DatabaseManager::getTextBean(const std::string& text_id, const std::string& text_type) const {
	TextBean bean;
	sqlite3_stmt *statement;
	std::string query = "SELECT * FROM text WHERE text_id = '" + 
		text_id + "' AND text_type = '" + text_type + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 5) {
			g_logger->logError("DatabaseManager::getTextBean", "number of returned columns must be 5");
			return bean;
		}
		int result = 0;
		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				bean.text_id = std::string((char*)sqlite3_column_text(statement, 0));
				bean.text_type = std::string((char*)sqlite3_column_text(statement, 1));
				bean.english = std::string((char*)sqlite3_column_text(statement, 2));
				bean.german = std::string((char*)sqlite3_column_text(statement, 3));
				bean.swiss_german = std::string((char*)sqlite3_column_text(statement, 4));
				bean.status = BeanStatus::Filled;
			}
			else {
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

ItemAttributeBean DatabaseManager::getItemAttributeBean(const std::string& item_id) const {
	ItemAttributeBean bean;
	sqlite3_stmt *statement;
	std::string query = "SELECT * FROM item_attribute WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 16) {
			g_logger->logError("DatabaseManager::getItemAttributeBean", "number of returned columns must be 16");
			return bean;
		}
		int result = 0;
		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				bean.item_id = std::string((char*)sqlite3_column_text(statement, 0));
				bean.max_health = sqlite3_column_int(statement, 1);
				bean.health_regeneration = sqlite3_column_int(statement, 2);
				bean.haste = sqlite3_column_int(statement, 3);
				bean.critical = sqlite3_column_int(statement, 4);
				bean.heal = sqlite3_column_int(statement, 5);
				bean.dmg_physical = sqlite3_column_int(statement, 6);
				bean.dmg_fire = sqlite3_column_int(statement, 7);
				bean.dmg_ice = sqlite3_column_int(statement, 8);
				bean.dmg_shadow = sqlite3_column_int(statement, 9);
				bean.dmg_light = sqlite3_column_int(statement, 10);
				bean.res_physical = sqlite3_column_int(statement, 11);
				bean.res_fire = sqlite3_column_int(statement, 12);
				bean.res_ice = sqlite3_column_int(statement, 13);
				bean.res_shadow = sqlite3_column_int(statement, 14);
				bean.res_light = sqlite3_column_int(statement, 15);
				bean.status = BeanStatus::Filled;
			}
			else {
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

ItemBean DatabaseManager::getItemBean(const std::string& item_id) const {
	ItemBean bean;
	sqlite3_stmt *statement;
	std::string query = "SELECT * FROM item WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 5) {
			g_logger->logError("DatabaseManager::getItemBean", "number of returned columns must be 5");
			return bean;
		}
		int result = 0;
		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				bean.item_id = std::string((char*)sqlite3_column_text(statement, 0));
				bean.item_type = bean.resolveItemType(std::string((char*)sqlite3_column_text(statement, 1)));
				bean.icon_location.x = sqlite3_column_int(statement, 2);
				bean.icon_location.y = sqlite3_column_int(statement, 3);
				bean.gold_value = sqlite3_column_int(statement, 4);
				bean.status = BeanStatus::Filled;
			}
			else {
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

ItemConvertibleBean DatabaseManager::getItemConvertibleBean(const std::string& item_id) const {
	ItemConvertibleBean bean;
	sqlite3_stmt *statement;
	std::string query = "SELECT * FROM item_convertible WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 4) {
			g_logger->logError("DatabaseManager::getItemConvertibleBean", "number of returned columns must be 4");
			return bean;
		}
		int result = 0;
		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				bean.item_id = std::string((char*)sqlite3_column_text(statement, 0));
				bean.convertible_item_id = std::string((char*)sqlite3_column_text(statement, 1));
				bean.convertible_gold = sqlite3_column_int(statement, 2);
				bean.probability = sqlite3_column_int(statement, 3);
				if (bean.probability > 100) bean.probability = 100;
				if (bean.probability < 0) bean.probability = 0;
				bean.status = BeanStatus::Filled;
			}
			else {
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

ItemEquipmentBean DatabaseManager::getItemEquipmentBean(const std::string& item_id) const {
	ItemEquipmentBean bean;
	sqlite3_stmt *statement;
	std::string query = "SELECT * FROM item_equipment WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 2) {
			g_logger->logError("DatabaseManager::getItemEquipmentBean", "number of returned columns must be 2");
			return bean;
		}
		int result = 0;
		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				bean.item_id = std::string((char*)sqlite3_column_text(statement, 0));
				bean.texture_path = std::string((char*)sqlite3_column_text(statement, 1));
				bean.status = BeanStatus::Filled;
			}
			else {
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

ItemEquipmentLightBean DatabaseManager::getItemEquipmentLightBean(const std::string& item_id) const {
	ItemEquipmentLightBean bean;
	sqlite3_stmt *statement;
	std::string query = "SELECT * FROM item_equipment_light WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 6) {
			g_logger->logError("DatabaseManager::getItemEquipmentLightBean", "number of returned columns must be 6");
			return bean;
		}
		int result = 0;
		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				bean.item_id = std::string((char*)sqlite3_column_text(statement, 0));
				bean.light_offset.x = static_cast<float>(sqlite3_column_int(statement, 1));
				bean.light_offset.y = static_cast<float>(sqlite3_column_int(statement, 2));
				bean.light_radius.x = static_cast<float>(sqlite3_column_int(statement, 3));
				bean.light_radius.y = static_cast<float>(sqlite3_column_int(statement, 4));
				bean.brightness = static_cast<float>(sqlite3_column_double(statement, 5));
				bean.status = BeanStatus::Filled;
			}
			else {
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

LevelitemLightBean DatabaseManager::getLevelitemLightBean(const std::string& item_id) const {
	LevelitemLightBean bean;
	sqlite3_stmt *statement;
	std::string query = "SELECT * FROM levelitem_light WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 6) {
			g_logger->logError("DatabaseManager::getLevelitemLightBean", "number of returned columns must be 6");
			return bean;
		}
		int result = 0;
		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				bean.item_id = std::string((char*)sqlite3_column_text(statement, 0));
				bean.light_offset.x = static_cast<float>(sqlite3_column_int(statement, 1));
				bean.light_offset.y = static_cast<float>(sqlite3_column_int(statement, 2));
				bean.light_radius.x = static_cast<float>(sqlite3_column_int(statement, 3));
				bean.light_radius.y = static_cast<float>(sqlite3_column_int(statement, 4));
				bean.brightness = static_cast<float>(sqlite3_column_double(statement, 5));
				bean.status = BeanStatus::Filled;
			}
			else {
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

ItemFoodBean DatabaseManager::getItemFoodBean(const std::string& item_id) const {
	ItemFoodBean bean;
	sqlite3_stmt *statement;
	std::string query = "SELECT * FROM item_food WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 4) {
			g_logger->logError("DatabaseManager::getItemFoodBean", "number of returned columns must be 4");
			return bean;
		}
		int result = 0;
		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				bean.item_id = std::string((char*)sqlite3_column_text(statement, 0));
				bean.food_duration = sf::seconds(static_cast<float>(sqlite3_column_int(statement, 1)));
				bean.is_cookable = sqlite3_column_int(statement, 2) == 1;
				char* cookedItemId = (char*)sqlite3_column_text(statement, 3);
				if (cookedItemId != nullptr) {
					bean.cooked_item_id = std::string(cookedItemId);
				}
				bean.status = BeanStatus::Filled;
			}
			else {
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

ItemWeaponBean DatabaseManager::getItemWeaponBean(const std::string& item_id) const {
	ItemWeaponBean bean;
	sqlite3_stmt *statement;
	std::string query = "SELECT * FROM item_weapon WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 7) {
			g_logger->logError("DatabaseManager::getItemWeaponBean", "number of returned columns must be 7");
			return bean;
		}
		int result = 0;
		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				bean.item_id = std::string((char*)sqlite3_column_text(statement, 0));
				bean.chop_cooldown = sf::milliseconds(sqlite3_column_int(statement, 1));
				bean.chop_rect.left = static_cast<float>(sqlite3_column_int(statement, 2));
				bean.chop_rect.top = static_cast<float>(sqlite3_column_int(statement, 3));
				bean.chop_rect.width = static_cast<float>(sqlite3_column_int(statement, 4));
				bean.chop_rect.height = static_cast<float>(sqlite3_column_int(statement, 5));
				bean.chop_damage = sqlite3_column_int(statement, 6);
				bean.status = BeanStatus::Filled;
			}
			else {
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

ItemWeaponSlotBean DatabaseManager::getItemWeaponslotBean(const std::string& item_id, int slot_nr) const {
	ItemWeaponSlotBean bean;
	sqlite3_stmt *statement;
	std::string query = "SELECT * FROM item_weapon_slot WHERE item_id = '" + item_id + "' AND slot_nr = '" + std::to_string(slot_nr) + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 4) {
			g_logger->logError("DatabaseManager::getItemWeaponSlotBean", "number of returned columns must be 4");
			return bean;
		}
		int result = 0;
		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				bean.item_id = std::string((char*)sqlite3_column_text(statement, 0));
				bean.slot_nr = sqlite3_column_int(statement, 1);
				bean.slot_type = bean.resolveSlotType(std::string((char*)sqlite3_column_text(statement, 2)));
				bean.modifier_count = sqlite3_column_int(statement, 3);
				bean.status = BeanStatus::Filled;
			}
			else {
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

std::vector<ItemWeaponSlotBean> DatabaseManager::getItemWeaponSlotBeans(const std::string& item_id) const {
	std::vector<ItemWeaponSlotBean> beans;
	sqlite3_stmt *statement;
	std::string query = "SELECT * FROM item_weapon_slot WHERE item_id = '" + item_id + "' ORDER BY slot_nr ASC;";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 4) {
			g_logger->logError("DatabaseManager::getItemWeaponSlotBeans", "number of returned columns must be 4");
			return beans;
		}
		int result = 0;
		ItemWeaponSlotBean bean;
		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				bean.item_id = std::string((char*)sqlite3_column_text(statement, 0));
				bean.slot_nr = sqlite3_column_int(statement, 1);
				bean.slot_type = bean.resolveSlotType(std::string((char*)sqlite3_column_text(statement, 2)));
				bean.modifier_count = sqlite3_column_int(statement, 3);
				bean.status = BeanStatus::Filled;
				beans.push_back(bean);
			}
			else {
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return beans;
}

LevelitemBean DatabaseManager::getLevelitemBean(const std::string& item_id) const {
	LevelitemBean bean;
	sqlite3_stmt *statement;
	std::string query = "SELECT * FROM levelitem WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 6) {
			g_logger->logError("DatabaseManager::getLevelitemBean", "number of returned columns must be 6");
			return bean;
		}
		int result = 0;
		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				bean.item_id = std::string((char*)sqlite3_column_text(statement, 0));
				bean.sprite_offset.x = static_cast<float>(sqlite3_column_int(statement, 1));
				bean.sprite_offset.y = static_cast<float>(sqlite3_column_int(statement, 2));
				bean.bounding_box.x = static_cast<float>(sqlite3_column_int(statement, 3));
				bean.bounding_box.y = static_cast<float>(sqlite3_column_int(statement, 4));
				bean.frame_time = sf::milliseconds(sqlite3_column_int(statement, 5));
				bean.status = BeanStatus::Filled;
			}
			else {
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

LevelitemFrameBean DatabaseManager::getLevelitemFrameBean(const std::string& item_id, int frame_nr) const {
	LevelitemFrameBean bean;
	sqlite3_stmt *statement;
	std::string query = "SELECT * FROM levelitem_frame WHERE item_id = '" + item_id + "' AND frame_nr = '" + std::to_string(frame_nr) + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 6) {
			g_logger->logError("DatabaseManager::getLevelitemFrameBean", "number of returned columns must be 6");
			return bean;
		}
		int result = 0;
		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				bean.item_id = std::string((char*)sqlite3_column_text(statement, 0));
				bean.frame_nr = sqlite3_column_int(statement, 1);
				bean.texture_location.left = sqlite3_column_int(statement, 2);
				bean.texture_location.top = sqlite3_column_int(statement, 3);
				bean.texture_location.width = sqlite3_column_int(statement, 4);
				bean.texture_location.height = sqlite3_column_int(statement, 5);
				bean.status = BeanStatus::Filled;
			}
			else {
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

std::vector<LevelitemFrameBean> DatabaseManager::getLevelitemFrameBeans(const std::string& item_id) const {
	std::vector<LevelitemFrameBean> beans;
	sqlite3_stmt *statement;
	std::string query = "SELECT * FROM levelitem_frame WHERE item_id = '" + item_id + "' ORDER BY frame_nr ASC;";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 6) {
			g_logger->logError("DatabaseManager::getLevelitemFrameBeans", "number of returned columns must be 6");
			return beans;
		}
		int result = 0;
		LevelitemFrameBean bean;
		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				bean.item_id = std::string((char*)sqlite3_column_text(statement, 0));
				bean.frame_nr = sqlite3_column_int(statement, 1);
				bean.texture_location.left = sqlite3_column_int(statement, 2);
				bean.texture_location.top = sqlite3_column_int(statement, 3);
				bean.texture_location.width = sqlite3_column_int(statement, 4); 
				bean.texture_location.height = sqlite3_column_int(statement, 5);
				bean.status = BeanStatus::Filled;
				beans.push_back(bean);
			}
			else {
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return beans;
}

bool DatabaseManager::itemExists(const std::string& item_id) const {
	sqlite3_stmt *statement;
	std::string query = "SELECT count(*) FROM item WHERE item_id = '" + item_id + "';";
	bool exists = false;

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 1) {
			g_logger->logError("DatabaseManager::itemExists", "number of returned columns must be 1");
			return false;
		}
		int result = 0;
		LevelitemFrameBean bean;
		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				exists = sqlite3_column_int(statement, 0) > 0;
			}
			else {
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return exists;
}

