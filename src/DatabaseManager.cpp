#include "DatabaseManager.h"

#include "Beans/ItemAttributeBean.h"
#include "Beans/ItemBean.h"
#include "Beans/ItemEquipmentBean.h"
#include "Beans/ItemConvertibleBean.h"
#include "Beans/ItemSpellBean.h"
#include "Beans/ItemEquipmentLightBean.h"
#include "Beans/ItemFoodBean.h"
#include "Beans/ItemWeaponBean.h"
#include "Beans/ItemWeaponSlotBean.h"
#include "Beans/ItemDocumentPageBean.h"
#include "Beans/ItemDocumentQuestBean.h"
#include "Beans/LevelitemBean.h"
#include "Beans/LevelitemFrameBean.h"
#include "Beans/LevelitemLightBean.h"
#include "Beans/SpawnBean.h"

DatabaseManager *g_databaseManager;

DatabaseManager::DatabaseManager() {
	init();
}

DatabaseManager::~DatabaseManager() {
	if (m_db != nullptr)
		sqlite3_close(m_db);
}

void DatabaseManager::init() {
	int rc = sqlite3_open(getResourcePath(DB_FILENAME).c_str(), &m_db);

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
	sqlite3_stmt* statement;
	ResultSet results;

	if (sqlite3_prepare_v2(m_db, query_.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		while (true) {
			if (sqlite3_step(statement) == SQLITE_ROW) {
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

ItemAttributeBean* DatabaseManager::getItemAttributeBean(const std::string& item_id) const {
	ItemAttributeBean* bean = nullptr;
	sqlite3_stmt* statement;
	std::string query = "SELECT * FROM item_attribute WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 16) {
			g_logger->logError("DatabaseManager::getItemAttributeBean", "number of returned columns must be 16");
			return bean;
		}

		if (sqlite3_step(statement) == SQLITE_ROW) {
			bean = new ItemAttributeBean();
			bean->item_id = std::string((char*)sqlite3_column_text(statement, 0));
			bean->max_health = sqlite3_column_int(statement, 1);
			bean->health_regeneration = sqlite3_column_int(statement, 2);
			bean->haste = sqlite3_column_int(statement, 3);
			bean->critical = sqlite3_column_int(statement, 4);
			bean->heal = sqlite3_column_int(statement, 5);
			bean->dmg_physical = sqlite3_column_int(statement, 6);
			bean->dmg_fire = sqlite3_column_int(statement, 7);
			bean->dmg_ice = sqlite3_column_int(statement, 8);
			bean->dmg_shadow = sqlite3_column_int(statement, 9);
			bean->dmg_light = sqlite3_column_int(statement, 10);
			bean->res_physical = sqlite3_column_int(statement, 11);
			bean->res_fire = sqlite3_column_int(statement, 12);
			bean->res_ice = sqlite3_column_int(statement, 13);
			bean->res_shadow = sqlite3_column_int(statement, 14);
			bean->res_light = sqlite3_column_int(statement, 15);
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

ItemBean* DatabaseManager::getItemBean(const std::string& item_id) const {
	ItemBean* bean = nullptr;
	sqlite3_stmt* statement;
	std::string query = "SELECT * FROM item WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 5) {
			g_logger->logError("DatabaseManager::getItemBean", "number of returned columns must be 5");
			return bean;
		}

		if (sqlite3_step(statement) == SQLITE_ROW) {
			bean = new ItemBean();
			bean->item_id = std::string((char*)sqlite3_column_text(statement, 0));
			bean->item_type = bean->resolveItemType(std::string((char*)sqlite3_column_text(statement, 1)));
			bean->icon_location.x = sqlite3_column_int(statement, 2);
			bean->icon_location.y = sqlite3_column_int(statement, 3);
			bean->gold_value = sqlite3_column_int(statement, 4);
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

std::vector<DatabaseBean*> DatabaseManager::getItemConvertibleBeans(const std::string& item_id) const {
	std::vector<DatabaseBean*> beans;
	sqlite3_stmt* statement;
	std::string query = "SELECT * FROM item_convertible WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 4) {
			g_logger->logError("DatabaseManager::getItemConvertibleBeans", "number of returned columns must be 4");
			return beans;
		}

		while (true) {
			if (sqlite3_step(statement) == SQLITE_ROW) {
				ItemConvertibleBean* bean = new ItemConvertibleBean();
				bean->item_id = std::string((char*)sqlite3_column_text(statement, 0));
				bean->convertible_item_id = std::string((char*)sqlite3_column_text(statement, 1));
				bean->convertible_amount = sqlite3_column_int(statement, 2);
				if (bean->convertible_amount < 1) bean->convertible_amount = 1;
				bean->probability = sqlite3_column_int(statement, 3);
				if (bean->probability > 100) bean->probability = 100;
				if (bean->probability < 0) bean->probability = 0;
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

ItemSpellBean* DatabaseManager::getItemSpellBean(const std::string& item_id) const {
	ItemSpellBean* bean = nullptr;
	sqlite3_stmt* statement;
	std::string query = "SELECT * FROM item_spell WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 2) {
			g_logger->logError("DatabaseManager::getItemSpellBean", "number of returned columns must be 2");
			return bean;
		}

		if (sqlite3_step(statement) == SQLITE_ROW) {
			bean = new ItemSpellBean();
			bean->item_id = std::string((char*)sqlite3_column_text(statement, 0));
			bean->spell_id = sqlite3_column_int(statement, 1);
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

ItemEquipmentBean* DatabaseManager::getItemEquipmentBean(const std::string& item_id) const {
	ItemEquipmentBean* bean = nullptr;
	sqlite3_stmt* statement;
	std::string query = "SELECT * FROM item_equipment WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 9) {
			g_logger->logError("DatabaseManager::getItemEquipmentBean", "number of returned columns must be 8");
			return bean;
		}

		if (sqlite3_step(statement) == SQLITE_ROW) {
			bean = new ItemEquipmentBean();
			bean->item_id = std::string((char*)sqlite3_column_text(statement, 0));
			bean->texture_path = std::string((char*)sqlite3_column_text(statement, 1));
			bean->map_texture_path = std::string((char*)sqlite3_column_text(statement, 2));
			bean->frames_walk = sqlite3_column_int(statement, 3);
			bean->frames_idle = sqlite3_column_int(statement, 4);
			bean->frames_jump = sqlite3_column_int(statement, 5);
			bean->frames_fight = sqlite3_column_int(statement, 6);
			bean->frames_climb1 = sqlite3_column_int(statement, 7);
			bean->frames_climb2 = sqlite3_column_int(statement, 8);
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

ItemEquipmentLightBean* DatabaseManager::getItemEquipmentLightBean(const std::string& item_id) const {
	ItemEquipmentLightBean* bean = nullptr;
	sqlite3_stmt* statement;
	std::string query = "SELECT * FROM item_equipment_light WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 8) {
			g_logger->logError("DatabaseManager::getItemEquipmentLightBean", "number of returned columns must be 8");
			return bean;
		}

		if (sqlite3_step(statement) == SQLITE_ROW) {
			bean = new ItemEquipmentLightBean();
			bean->item_id = std::string((char*)sqlite3_column_text(statement, 0));
			bean->light_offset.x = static_cast<float>(sqlite3_column_int(statement, 1));
			bean->light_offset.y = static_cast<float>(sqlite3_column_int(statement, 2));
			bean->light_radius.x = static_cast<float>(sqlite3_column_int(statement, 3));
			bean->light_radius.y = static_cast<float>(sqlite3_column_int(statement, 4));
			bean->map_light_radius.x = static_cast<float>(sqlite3_column_int(statement, 5));
			bean->map_light_radius.y = static_cast<float>(sqlite3_column_int(statement, 6));
			bean->brightness = static_cast<float>(sqlite3_column_double(statement, 7));
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

LevelitemLightBean* DatabaseManager::getLevelitemLightBean(const std::string& item_id) const {
	LevelitemLightBean* bean = new LevelitemLightBean();
	sqlite3_stmt* statement;
	std::string query = "SELECT * FROM levelitem_light WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 6) {
			g_logger->logError("DatabaseManager::getLevelitemLightBean", "number of returned columns must be 6");
			return bean;
		}

		if (sqlite3_step(statement) == SQLITE_ROW) {
			bean = new LevelitemLightBean();
			bean->item_id = std::string((char*)sqlite3_column_text(statement, 0));
			bean->light_offset.x = static_cast<float>(sqlite3_column_int(statement, 1));
			bean->light_offset.y = static_cast<float>(sqlite3_column_int(statement, 2));
			bean->light_radius.x = static_cast<float>(sqlite3_column_int(statement, 3));
			bean->light_radius.y = static_cast<float>(sqlite3_column_int(statement, 4));
			bean->brightness = static_cast<float>(sqlite3_column_double(statement, 5));
		}


		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

ItemFoodBean* DatabaseManager::getItemFoodBean(const std::string& item_id) const {
	ItemFoodBean* bean = nullptr;
	sqlite3_stmt* statement;
	std::string query = "SELECT * FROM item_food WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 4) {
			g_logger->logError("DatabaseManager::getItemFoodBean", "number of returned columns must be 4");
			return bean;
		}

		if (sqlite3_step(statement) == SQLITE_ROW) {
			bean = new ItemFoodBean();
			bean->item_id = std::string((char*)sqlite3_column_text(statement, 0));
			bean->food_duration = sf::seconds(static_cast<float>(sqlite3_column_int(statement, 1)));
			bean->is_cookable = sqlite3_column_int(statement, 2) == 1;
			char* cookedItemId = (char*)sqlite3_column_text(statement, 3);
			if (cookedItemId != nullptr) {
				bean->cooked_item_id = std::string(cookedItemId);
			}
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

ItemWeaponBean* DatabaseManager::getItemWeaponBean(const std::string& item_id) const {
	ItemWeaponBean* bean = nullptr;
	sqlite3_stmt* statement;
	std::string query = "SELECT * FROM item_weapon WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 7) {
			g_logger->logError("DatabaseManager::getItemWeaponBean", "number of returned columns must be 7");
			return bean;
		}
		if (sqlite3_step(statement) == SQLITE_ROW) {
			bean = new ItemWeaponBean();
			bean->item_id = std::string((char*)sqlite3_column_text(statement, 0));
			bean->chop_cooldown = sf::milliseconds(sqlite3_column_int(statement, 1));
			bean->chop_rect.left = static_cast<float>(sqlite3_column_int(statement, 2));
			bean->chop_rect.top = static_cast<float>(sqlite3_column_int(statement, 3));
			bean->chop_rect.width = static_cast<float>(sqlite3_column_int(statement, 4));
			bean->chop_rect.height = static_cast<float>(sqlite3_column_int(statement, 5));
			bean->chop_damage = sqlite3_column_int(statement, 6);
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

ItemWeaponSlotBean* DatabaseManager::getItemWeaponslotBean(const std::string& item_id, int slot_nr) const {
	ItemWeaponSlotBean* bean = nullptr;
	sqlite3_stmt* statement;
	std::string query = "SELECT * FROM item_weapon_slot WHERE item_id = '" + item_id + "' AND slot_nr = '" + std::to_string(slot_nr) + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 4) {
			g_logger->logError("DatabaseManager::getItemWeaponSlotBean", "number of returned columns must be 4");
			return bean;
		}

		if (sqlite3_step(statement) == SQLITE_ROW) {
			bean = new ItemWeaponSlotBean();
			bean->item_id = std::string((char*)sqlite3_column_text(statement, 0));
			bean->slot_nr = sqlite3_column_int(statement, 1);
			bean->slot_type = bean->resolveSlotType(std::string((char*)sqlite3_column_text(statement, 2)));
			bean->modifier_count = sqlite3_column_int(statement, 3);
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

std::vector<DatabaseBean*> DatabaseManager::getItemDocumentPageBeans(const std::string& item_id) const {
	std::vector<DatabaseBean*> beans;
	sqlite3_stmt* statement;
	std::string query = "SELECT * FROM item_document_page WHERE item_id = '" + item_id + "' ORDER BY page_nr ASC;";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 6) {
			g_logger->logError("DatabaseManager::getItemDocumentPageBeans", "number of returned columns must be 6");
			return beans;
		}
		while (true) {
			if (sqlite3_step(statement) == SQLITE_ROW) {
				ItemDocumentPageBean* bean = new ItemDocumentPageBean();
				bean->item_id = std::string((char*)sqlite3_column_text(statement, 0));
				bean->page_nr = sqlite3_column_int(statement, 1);
				bean->title = std::string((char*)sqlite3_column_text(statement, 2));
				bean->content = std::string((char*)sqlite3_column_text(statement, 3));
				bean->texture_path = std::string((char*)sqlite3_column_text(statement, 4));
				bean->content_alignment = resolveTextAlignment((char*)sqlite3_column_text(statement, 5));
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

ItemDocumentQuestBean* DatabaseManager::getItemDocumentQuestBean(const std::string& item_id) const {
	ItemDocumentQuestBean* bean = nullptr;
	sqlite3_stmt* statement;
	std::string query = "SELECT * FROM item_document_quest WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 4) {
			g_logger->logError("DatabaseManager::getItemDocumentQuestBean", "number of returned columns must be 4");
			return bean;
		}
		if (sqlite3_step(statement) == SQLITE_ROW) {
			bean = new ItemDocumentQuestBean();
			bean->item_id = std::string((char*)sqlite3_column_text(statement, 0));
			bean->quest_name = std::string((char*)sqlite3_column_text(statement, 1));
			bean->quest_state = std::string((char*)sqlite3_column_text(statement, 2));
			bean->quest_desc = sqlite3_column_int(statement, 3);
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

std::vector<DatabaseBean*> DatabaseManager::getItemWeaponSlotBeans(const std::string& item_id) const {
	std::vector<DatabaseBean*> beans;
	sqlite3_stmt* statement;
	std::string query = "SELECT * FROM item_weapon_slot WHERE item_id = '" + item_id + "' ORDER BY slot_nr ASC;";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 4) {
			g_logger->logError("DatabaseManager::getItemWeaponSlotBeans", "number of returned columns must be 4");
			return beans;
		}

		while (true) {
			if (sqlite3_step(statement) == SQLITE_ROW) {
				ItemWeaponSlotBean* bean = new ItemWeaponSlotBean();
				bean->item_id = std::string((char*)sqlite3_column_text(statement, 0));
				bean->slot_nr = sqlite3_column_int(statement, 1);
				bean->slot_type = bean->resolveSlotType(std::string((char*)sqlite3_column_text(statement, 2)));
				bean->modifier_count = sqlite3_column_int(statement, 3);
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

LevelitemBean* DatabaseManager::getLevelitemBean(const std::string& item_id) const {
	LevelitemBean* bean = nullptr;
	sqlite3_stmt* statement;
	std::string query = "SELECT * FROM levelitem WHERE item_id = '" + item_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 6) {
			g_logger->logError("DatabaseManager::getLevelitemBean", "number of returned columns must be 6");
			return bean;
		}
		if (sqlite3_step(statement) == SQLITE_ROW) {
			bean = new LevelitemBean();
			bean->item_id = std::string((char*)sqlite3_column_text(statement, 0));
			bean->sprite_offset.x = static_cast<float>(sqlite3_column_int(statement, 1));
			bean->sprite_offset.y = static_cast<float>(sqlite3_column_int(statement, 2));
			bean->bounding_box.x = static_cast<float>(sqlite3_column_int(statement, 3));
			bean->bounding_box.y = static_cast<float>(sqlite3_column_int(statement, 4));
			bean->frame_time = sf::milliseconds(sqlite3_column_int(statement, 5));
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

LevelitemFrameBean* DatabaseManager::getLevelitemFrameBean(const std::string& item_id, int frame_nr) const {
	LevelitemFrameBean* bean = nullptr;
	sqlite3_stmt* statement;
	std::string query = "SELECT * FROM levelitem_frame WHERE item_id = '" + item_id + "' AND frame_nr = '" + std::to_string(frame_nr) + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 6) {
			g_logger->logError("DatabaseManager::getLevelitemFrameBean", "number of returned columns must be 6");
			return bean;
		}
		if (sqlite3_step(statement) == SQLITE_ROW) {
			bean = new LevelitemFrameBean();
			bean->item_id = std::string((char*)sqlite3_column_text(statement, 0));
			bean->frame_nr = sqlite3_column_int(statement, 1);
			bean->texture_location.left = sqlite3_column_int(statement, 2);
			bean->texture_location.top = sqlite3_column_int(statement, 3);
			bean->texture_location.width = sqlite3_column_int(statement, 4);
			bean->texture_location.height = sqlite3_column_int(statement, 5);
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}

std::vector<DatabaseBean*> DatabaseManager::getLevelitemFrameBeans(const std::string& item_id) const {
	std::vector<DatabaseBean*> beans;
	sqlite3_stmt* statement;
	std::string query = "SELECT * FROM levelitem_frame WHERE item_id = '" + item_id + "' ORDER BY frame_nr ASC;";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 6) {
			g_logger->logError("DatabaseManager::getLevelitemFrameBeans", "number of returned columns must be 6");
			return beans;
		}
		while (true) {
			if (sqlite3_step(statement) == SQLITE_ROW) {
				LevelitemFrameBean* bean = new LevelitemFrameBean();
				bean->item_id = std::string((char*)sqlite3_column_text(statement, 0));
				bean->frame_nr = sqlite3_column_int(statement, 1);
				bean->texture_location.left = sqlite3_column_int(statement, 2);
				bean->texture_location.top = sqlite3_column_int(statement, 3);
				bean->texture_location.width = sqlite3_column_int(statement, 4);
				bean->texture_location.height = sqlite3_column_int(statement, 5);
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
	sqlite3_stmt* statement;
	std::string query = "SELECT count(*) FROM item WHERE item_id = '" + item_id + "';";
	bool exists = false;

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 1) {
			g_logger->logError("DatabaseManager::itemExists", "number of returned columns must be 1");
			return false;
		}
		LevelitemFrameBean bean;
		while (true) {
			if (sqlite3_step(statement) == SQLITE_ROW) {
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

SpawnBean* DatabaseManager::getSpawnBean(const std::string& spawn_id) const {
	SpawnBean* bean = nullptr;
	sqlite3_stmt* statement;
	std::string query = "SELECT * FROM spawn WHERE spawn_id = '" + spawn_id + "';";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		if (cols != 6) {
			g_logger->logError("DatabaseManager::getSpawnBean", "number of returned columns must be 6");
			return bean;
		}
		if (sqlite3_step(statement) == SQLITE_ROW) {
			bean = new SpawnBean();
			bean->spawn_id = std::string((char*)sqlite3_column_text(statement, 0));
			bean->map_id = std::string((char*)sqlite3_column_text(statement, 1));
			bean->map_pos.x = static_cast<float>(sqlite3_column_int(statement, 2));
			bean->map_pos.y = static_cast<float>(sqlite3_column_int(statement, 3));
			bean->weapon_id = std::string((char*)sqlite3_column_text(statement, 4));
			bean->armor_id = std::string((char*)sqlite3_column_text(statement, 5));
		}

		sqlite3_finalize(statement);
	}

	checkError();

	return bean;
}