#include "FileIO/ItemReader.h"
#include "ResourceManager.h"

using namespace std;

const int COLUMN_COUNT = 33;

ItemReader::ItemReader() {
}

ItemReader::~ItemReader() {
}

bool ItemReader::readItems(std::map<std::string, ItemBean>& itemMap) const {
	itemMap.clear();
	wstring contents = getFileContentsWide(g_resourceManager->getFilename(ResourceID::Items));
	if (contents.empty()) {
		return false;
	}
	StringTable tab;
	parseCsv((wchar_t *)contents.c_str(), tab);
	if (tab.size() == 0 || tab[0].size() != COLUMN_COUNT) {
		g_logger->logError("ItemReader", "Error in item file, incorrect number of columns or no rows");
		return false;
	}

	int lineNr = 0;
	for (auto &row : tab) {
		lineNr++;
		if (lineNr == 1) {
			// we skip the first row as it is a description of the colums (title row)
			continue;
		}
		ItemBean item = DEFAULT_ITEM;
		vector<string> columns;
		for (auto &col : row) {
			columns.push_back(string(col.begin(), col.end()));
		}

		while (columns.size() < COLUMN_COUNT) {
			columns.push_back("");
		}

		if (columns.size() != COLUMN_COUNT) {
			g_logger->logError("ItemReader", "Error in item file, incorrect number of columns");
			return false;
		}

		// <<<<<<<< PARSE >>>>>>>>>

		// item id
		item.id = columns[0];
		// item description
		item.description = columns[1];
		// item type
		item.type = static_cast<ItemType>(atoi(columns[2].c_str()));
		// icon texture location
		if (!columns[3].empty() && columns[3].find(",") != string::npos && columns[3].find(",") + 1 < columns[3].size()) {
			item.iconTextureLocation.x = atoi(columns[3].c_str());
			item.iconTextureLocation.y = atoi(columns[3].substr(columns[3].find(",") + 1).c_str());
		}
		// gold value
		item.goldValue = atoi(columns[4].c_str());
		// attributes
		item.attributes.maxHealthPoints = atoi(columns[5].c_str());
		item.attributes.healthRegenerationPerS = atoi(columns[6].c_str());
		item.attributes.haste = atoi(columns[7].c_str());
		item.attributes.critical = atoi(columns[8].c_str());
		item.attributes.damagePhysical = atoi(columns[9].c_str());
		item.attributes.damageFire = atoi(columns[10].c_str());
		item.attributes.damageIce = atoi(columns[11].c_str());
		item.attributes.damageShadow = atoi(columns[12].c_str());
		item.attributes.damageLight = atoi(columns[13].c_str());
		item.attributes.resistancePhysical = atoi(columns[14].c_str());
		item.attributes.resistanceFire = atoi(columns[15].c_str());
		item.attributes.resistanceIce = atoi(columns[16].c_str());
		item.attributes.resistanceShadow = atoi(columns[17].c_str());
		item.attributes.resistanceLight = atoi(columns[18].c_str());
		// food duration
		item.foodDuration = sf::seconds(static_cast<float>(atoi(columns[19].c_str())));
		// sprite offset
		if (!columns[20].empty() && columns[20].find(",") != string::npos && columns[20].find(",") + 1 < columns[20].size()) {
			item.spriteOffset.x = static_cast<float>(atoi(columns[20].c_str()));
			item.spriteOffset.y = static_cast<float>(atoi(columns[20].substr(columns[20].find(",") + 1).c_str()));
		}
		// level item bounding box
		if (!columns[21].empty()) {
			std::stringstream ss(columns[21]);
			int i;
			vector<float> boundingBoxValues;
			while (ss >> i) {
				boundingBoxValues.push_back(static_cast<float>(i));

				if (ss.peek() == ',' || ss.peek() == ' ')
					ss.ignore();
			}
			if (boundingBoxValues.size() != 2) {
				g_logger->logError("ItemReader", "Level Item Bounding box could not be parsed!");
				return false;
			}
			item.boundingBox.left = 0;
			item.boundingBox.top = 0;
			item.boundingBox.width = boundingBoxValues[0];
			item.boundingBox.height = boundingBoxValues[1];
		}
		// level item texture position(s)
		if (!columns[22].empty()) {
			std::stringstream ss(columns[22]);
			int i;
			vector<int> texturePositions;
			while (ss >> i) {
				texturePositions.push_back(i);

				if (ss.peek() == ',' || ss.peek() == ' ')
					ss.ignore();
			}
			if (texturePositions.size() % 4 != 0) {
				g_logger->logError("ItemReader", "Level Item texture positions could not be parsed!");
				return false;
			}

			for (int val = 0; val < texturePositions.size();) {
				sf::IntRect texturePosition;
				texturePosition.left = texturePositions[val++];
				texturePosition.top = texturePositions[val++];
				texturePosition.width = texturePositions[val++];
				texturePosition.height = texturePositions[val++];
				item.texturePositions.push_back(texturePosition);
			}
		}
		// frame time
		if (!columns[23].empty()) {
			item.frameTime = sf::milliseconds(atoi(columns[23].c_str()));
		}
		// path to equipment texture
		item.spritesheetPath = columns[24];
		// weapon chop cooldown
		if (!columns[25].empty()) {
			item.weaponChopCooldown = sf::milliseconds(atoi(columns[25].c_str()));
		}
		// weapon chop bounding box
		if (!columns[26].empty()) {
			std::stringstream ss(columns[26]);
			int i;
			vector<float> boundingBoxValues;
			while (ss >> i) {
				boundingBoxValues.push_back(static_cast<float>(i));

				if (ss.peek() == ',' || ss.peek() == ' ')
					ss.ignore();
			}
			if (boundingBoxValues.size() != 2) {
				g_logger->logError("ItemReader", "Weapon Chop Bounding box could not be parsed!");
				return false;
			}
			item.weaponChopRect.left = 0;
			item.weaponChopRect.top = 0;
			item.weaponChopRect.width = boundingBoxValues[0];
			item.weaponChopRect.height = boundingBoxValues[1];
		}
		// weapon chop damage
		if (!columns[27].empty()) {
			item.weaponChopDamage = atoi(columns[27].c_str());
		}
		// slot 1-5
		for (int col = 28; col < 28 + 5; col++) {
			if (columns[col].empty()) break;

			std::stringstream ss(columns[col]);
			int i;
			vector<int> slotValues;
			while (ss >> i) {
				slotValues.push_back(i);

				if (ss.peek() == ',' || ss.peek() == ' ')
					ss.ignore();
			}
			if (slotValues.size() != 2) {
				g_logger->logError("ItemReader", "Weapon Spell Slot could not be parsed!");
				return false;
			}
			WeaponSpellSlotBean slot;
			slot.type = static_cast<SpellType>(slotValues[0]);
			slot.modifierCount = slotValues[1];

			item.weaponSlots.push_back(slot);
		}

		if (!checkItem(item)) {
			return false;
		}

		itemMap.insert({ item.id, item });
	}

	return true;
}

bool ItemReader::checkItem(const ItemBean& bean) const {
	if (bean.id.empty()) {
		return false;
	}
	if (bean.description.empty()) {
		return false;
	}
	if (bean.type <= ItemType::VOID || bean.type >= ItemType::MAX) {
		return false;
	}
	if (bean.type != ItemType::Equipment_weapon) {
		if (bean.weaponChopCooldown != sf::Time::Zero ||
			bean.weaponSlots.size() > 0 ||
			bean.weaponChopDamage != 0 ||
			bean.weaponChopRect.height != 0 || bean.weaponChopRect.width != 0) {
			return false;
		}
	}
	else {
		if (bean.weaponSlots.size() < 0 || bean.weaponSlots.size() > 5) {
			return false;
		}
		for (auto& it : bean.weaponSlots) {
			if (it.type <= SpellType::VOID || it.type >= SpellType::MAX) {
				g_logger->logError("ItemReader", "Weapon slot spell type not resolved.");
				return false;
			}
			if (it.modifierCount < 0 || it.modifierCount > 3) {
				g_logger->logError("ItemReader", "Weapon slot modifier count not allowed! Must be between 0 and 3");
				return false;
			}
		}
	}

	return true;
}