#include "World/ChestTile.h"
#include "Logger.h"

bool ChestTile::init(const std::map<std::string, std::string>& properties) {
	for (auto& it : properties) {
		if (it.first == "permanent") {
			m_isPermanent = true;
		}
		else if (it.first == "open") {
			m_isOpen = true;
		}
		else if (it.first == "storeditems") {
			m_isStoredItems = true;
		}
		else if (it.first == "text") {
			m_tooltipText = it.second;
		}
		else if (it.first == "key") {
			m_keyItemID = it.second;
		}
		else if (it.first == "luapath") {
			m_luapath = it.second;
		}
		else if (it.first == "light") {
			if (!LightData::resolveLightString(it.second, m_lightData))
				return false;
		}
		else if (it.first == "gold") {
			m_lootableGold = std::stoi(it.second);
		}
		else if (it.first == "strength") {
			m_chestStrength = std::stoi(it.second);
			if (m_chestStrength > 5 || m_chestStrength < 0) {
				m_chestStrength = 0;
			}
		}
		else {
			m_lootableItems.insert({ it.first, std::stoi(it.second) });
		}
	}

	if (m_chestStrength == 0 && !m_keyItemID.empty()) {
		g_logger->logError("ChestTile", "cannot add chest that requires key but lock strength is 0");
		return false;
	}
	if (m_chestStrength > 4 && m_keyItemID.empty()) {
		g_logger->logError("ChestTile", "cannot add chest with strength > 4 that has no key");
		return false;
	}

	return true;
}


