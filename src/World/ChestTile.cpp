#include "World/ChestTile.h"
#include "Logger.h"

bool ChestTile::init(const std::map<std::string, std::string>& properties) {
	for (auto& it : properties) {
		if (it.first.compare("permanent") == 0) {
			m_isPermanent = true;
		}
		else if (it.first.compare("open") == 0) {
			m_isOpen = true;
		}
		else if (it.first.compare("storeditems") == 0) {
			m_isStoredItems = true;
		}
		else if (it.first.compare("text") == 0) {
			m_tooltipText = it.second;
		}
		else if (it.first.compare("key") == 0) {
			m_keyItemID = it.second;
		}
		else if (it.first.compare("luapath") == 0) {
			m_luapath = it.second;
		}
		else if (it.first.compare("light") == 0) {
			if (!LightData::resolveLightString(it.second, m_lightData))
				return false;
		}
		else if (it.first.compare("gold") == 0) {
			m_lootableGold = std::stoi(it.second);
		}
		else if (it.first.compare("strength") == 0) {
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


