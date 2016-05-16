#include "FileIO/CharacterCoreWriter.h"
#include "Enums/EnumNames.h"

using namespace std;

bool CharacterCoreWriter::createFile(const std::string& filename) const {
	if (std::ifstream(filename)) {
		g_logger->logInfo("CharacterCoreWriter", "File already exists: " + filename);
		return false;
	}
	std::ofstream file(filename);
	g_logger->logInfo("CharacterCoreWriter", "Created new file: " + filename);
	if (!file) {
		g_logger->logError("CharacterCoreWriter", "File could not be created: " + filename);
		return false;
	}
	return true;
}

bool CharacterCoreWriter::saveToFile(const std::string& filename, const CharacterCoreData& data) const {
	ofstream savefile(filename, ios::trunc);
	if (savefile.is_open()) {
		savefile << writeTimePlayed(data);
		savefile << writeSaveGameName(data);
		savefile << writeDateSaved(data);
		savefile << writeLevelID(data);
		savefile << writeLevelPosition(data);
		savefile << writeIsInLevel(data);
		savefile << writeMapID(data);
		savefile << writeMapPosition(data);
		savefile << writeAttributes(data);
		savefile << writeGold(data);
		savefile << writeItemID(data);
		savefile << writeEquippedWeaponSlots(data);
		savefile << writeEquippedItems(data);
		savefile << writeQuickslots(data);
		savefile << writeEnemiesKilled(data);
		savefile << writeEnemiesLooted(data);
		savefile << writeItemsLooted(data);
		savefile << writeChestsLooted(data);
		savefile << writeWaypointsUnlocked(data);
		savefile << writeTriggersTriggered(data);
		savefile << writeTilesExplored(data);
		savefile << writeQuestStates(data);
		savefile << writeQuestProgressTargets(data);
		savefile << writeQuestProgressConditions(data);
		savefile << writeQuestProgressDescription(data);
		savefile << writeProgressConditions(data);
		savefile << writeReputationProgress(data);
		savefile << writeMerchandStates(data);
		savefile << writeSpellsLearned(data);
		savefile << writeModifiersLearned(data);
		savefile << writeHintsLearned(data);
		savefile << writeWeather(data);

		savefile.close();
	}
	else {
		g_logger->logError("CharacterCoreWriter", "Unable to open file: " + string(filename));
		return false;
	}
	return true;
}

std::string CharacterCoreWriter::writeTimePlayed(const CharacterCoreData& data) const {
	string timePlayed = "# time played, in seconds\n";
	return timePlayed.append(string(TIME_PLAYED) + ":" + to_string(static_cast<int>(std::floor(data.timePlayed.asSeconds()))) + "\n");
}

std::string CharacterCoreWriter::writeSaveGameName(const CharacterCoreData& data) const {
	return string(SAVE_GAME_NAME) + ":" + data.saveGameName + "\n";
}

std::string CharacterCoreWriter::writeDateSaved(const CharacterCoreData& data) const {
	return string(DATE_SAVED) + ":" + to_string(data.dateSaved) + "\n";
}

std::string CharacterCoreWriter::writeMapID(const CharacterCoreData& data) const {
	return string(MAP_ID) + ":" + data.currentMap + "\n";
}

std::string CharacterCoreWriter::writeIsInLevel(const CharacterCoreData& data) const {
	return string(IS_IN_LEVEL) + ":" + (data.isInLevel ? "1" : "0") + "\n";
}

std::string CharacterCoreWriter::writeMapPosition(const CharacterCoreData& data) const {
	return string(MAP_POSITION) + ":" + to_string(data.currentMapPosition.x) + "," + to_string(data.currentMapPosition.y) + "\n";
}

std::string CharacterCoreWriter::writeLevelID(const CharacterCoreData& data) const {
	return string(LEVEL_ID) + ":" + data.currentLevel + "\n";
}

std::string CharacterCoreWriter::writeLevelPosition(const CharacterCoreData& data) const {
	return string(LEVEL_POSITION) + ":" + to_string(data.currentLevelPosition.x) + "," + to_string(data.currentLevelPosition.y) + "\n";
}

std::string CharacterCoreWriter::writeEnemiesKilled(const CharacterCoreData& data) const {
	string enemiesKilled = "# enemies killed in level:\n";

	for (auto& it : data.enemiesKilled) {
		string enemyKilled = string(ENEMIES_KILLED);
		enemyKilled.append(":");
		enemyKilled.append(it.first);
		for (auto it2 : it.second) {
			enemyKilled.append("," + to_string(it2));
		}
		enemyKilled.append("\n");
		enemiesKilled.append(enemyKilled);
	}
	return enemiesKilled;
}

std::string CharacterCoreWriter::writeEnemiesLooted(const CharacterCoreData& data) const {
	string enemiesLooted = "# enemies looted in level:\n";

	for (auto& it : data.enemiesLooted) {
		string enemyLooted = string(ENEMIES_LOOTED);
		enemyLooted.append(":");
		enemyLooted.append(it.first);
		for (auto it2 : it.second) {
			enemyLooted.append("," + to_string(it2));
		}
		enemyLooted.append("\n");
		enemiesLooted.append(enemyLooted);
	}
	return enemiesLooted;
}

std::string CharacterCoreWriter::writeItemsLooted(const CharacterCoreData& data) const {
	string itemsLooted = "# items looted in level:\n";

	for (auto& it : data.itemsLooted) {
		string itemLooted = string(ITEMS_LOOTED);
		itemLooted.append(":");
		itemLooted.append(it.first);
		for (auto it2 : it.second) {
			itemLooted.append("," + to_string(it2));
		}
		itemLooted.append("\n");
		itemsLooted.append(itemLooted);
	}
	return itemsLooted;
}

std::string CharacterCoreWriter::writeWaypointsUnlocked(const CharacterCoreData& data) const {
	string waypointsUnlocked = "# waypoints unlocked in map:\n";

	for (auto& it : data.waypointsUnlocked) {
		string waypointUnlocked = string(WAYPOINTS_UNLOCKED);
		waypointUnlocked.append(":");
		waypointUnlocked.append(it.first);
		for (auto it2 : it.second) {
			waypointUnlocked.append("," + to_string(it2));
		}
		waypointUnlocked.append("\n");
		waypointsUnlocked.append(waypointUnlocked);
	}
	return waypointsUnlocked;
}

std::string CharacterCoreWriter::writeTriggersTriggered(const CharacterCoreData& data) const {
	string triggersTriggered = "# triggers triggered in world:\n";

	for (auto& it : data.triggersTriggered) {
		string triggerTriggered = string(TRIGGERS_TRIGGERED);
		triggerTriggered.append(":");
		triggerTriggered.append(it.first);
		for (auto it2 : it.second) {
			triggerTriggered.append("," + to_string(it2));
		}
		triggerTriggered.append("\n");
		triggersTriggered.append(triggerTriggered);
	}
	return triggersTriggered;
}

std::string CharacterCoreWriter::writeTilesExplored(const CharacterCoreData& data) const {
	string tilesExplored = "# tiles explored in world:\n";

	for (auto& it : data.tilesExplored) {
		string tiles = string(TILES_EXPLORED);
		tiles.append(":");
		tiles.append(it.first);
		for (auto it2 : it.second) {
			tiles.append("," + to_string(it2));
		}
		tiles.append("\n");
		tilesExplored.append(tiles);
	}
	return tilesExplored;
}

std::string CharacterCoreWriter::writeChestsLooted(const CharacterCoreData& data) const {
	string chestsLooted = "# chests looted in level:\n";

	for (auto& it : data.chestsLooted) {
		string chestLooted = string(ITEMS_LOOTED);
		chestLooted.append(":");
		chestLooted.append(it.first);
		for (auto it2 : it.second) {
			chestLooted.append("," + to_string(it2));
		}
		chestLooted.append("\n");
		chestsLooted.append(chestLooted);
	}
	return chestsLooted;
}

std::string CharacterCoreWriter::writeSpellsLearned(const CharacterCoreData& data) const {
	string spellsLearned = "# spells learned:\n";

	for (auto& it : data.spellsLearned) {
		string spellLearned = string(SPELL_LEARNED);
		spellLearned.append(":");
		spellLearned.append(std::to_string(static_cast<int>(it.first)));
		for (auto it2 : it.second) {
			spellLearned.append("," + to_string(static_cast<int>(it2)));
		}
		spellLearned.append("\n");
		spellsLearned.append(spellLearned);
	}
	return spellsLearned;
}

std::string CharacterCoreWriter::writeModifiersLearned(const CharacterCoreData& data) const {
	string modifiersLearned = "# modifiers learned:\n";

	for (auto& it : data.modfiersLearned) {
		string modifierLearned = string(MODIFIER_LEARNED);
		modifierLearned.append(":");
		modifierLearned.append(std::to_string(static_cast<int>(it.first)));
		modifierLearned.append(",");
		modifierLearned.append(std::to_string(it.second));
		modifierLearned.append("\n");
		modifiersLearned.append(modifierLearned);
	}
	return modifiersLearned;
}

std::string CharacterCoreWriter::writeAttributes(const CharacterCoreData& data) const {
	string orderComment = "# in this order: maxHealthPoints, healthRegenerationPerS, haste, critical, \n# damagePhysical, damageFire, damageIce, damageShadow, damageLight, \n# resistancePhysical, resistanceFire, resistanceIce, resistanceShadow, resistanceIce. \n";
	string attributeString = orderComment.append(string(ATTRIBUTES));
	attributeString.append(":");
	attributeString.append(to_string(data.attributes.maxHealthPoints) + ",");
	attributeString.append(to_string(data.attributes.healthRegenerationPerS) + ",");
	attributeString.append(to_string(data.attributes.haste) + ",");
	attributeString.append(to_string(data.attributes.critical) + ",");

	attributeString.append(to_string(data.attributes.damagePhysical) + ",");
	attributeString.append(to_string(data.attributes.damageFire) + ",");
	attributeString.append(to_string(data.attributes.damageIce) + ",");
	attributeString.append(to_string(data.attributes.damageShadow) + ",");
	attributeString.append(to_string(data.attributes.damageLight) + ",");

	attributeString.append(to_string(data.attributes.resistancePhysical) + ",");
	attributeString.append(to_string(data.attributes.resistanceFire) + ",");
	attributeString.append(to_string(data.attributes.resistanceIce) + ",");
	attributeString.append(to_string(data.attributes.resistanceShadow) + ",");
	attributeString.append(to_string(data.attributes.resistanceLight));
	attributeString.append("\n");

	return attributeString;
}

std::string CharacterCoreWriter::writeGold(const CharacterCoreData& data) const {
	return string(GOLD) + ":" + to_string(data.gold) + "\n";
}

std::string CharacterCoreWriter::writeQuestStates(const CharacterCoreData& data) const {
	string quests = "# quest states:\n";

	for (auto& it : data.questStates) {
		string quest = string(QUEST_STATE);
		quest.append(":");
		quest.append(it.first);
		quest.append(",");
		quest.append(to_string(static_cast<int>(it.second)));
		quest.append("\n");
		quests.append(quest);
	}
	return quests;
}

std::string CharacterCoreWriter::writeHintsLearned(const CharacterCoreData& data) const {
	string hintList = "# hints learned:\n";

	for (auto& it : data.hintsLearned) {
		string hint = string(HINT_LEARNED);
		hint.append(":");
		hint.append(it);
		hint.append("\n");
		hintList.append(hint);
	}

	return hintList;
}

std::string CharacterCoreWriter::writeReputationProgress(const CharacterCoreData& data) const {
	string reputationProgress = "# reputation progress:\n";

	for (auto& it : data.reputationProgress) {
		string reputation = string(REPUTATION_PROGRESS);
		reputation.append(":");
		reputation.append(EnumNames::getFractionIDName(it.first));
		reputation.append(",");
		reputation.append(to_string(it.second));
		reputation.append("\n");
		reputationProgress.append(reputation);
	}
	return reputationProgress;
}

std::string CharacterCoreWriter::writeWeather(const CharacterCoreData& data) const {
	string currentWeather = "# current weather:\n";

	for (auto& it : data.currentWeather) {
		string weather = string(WEATHER);
		weather.append(":");
		weather.append(it.first);
		weather.append(",");
		weather.append(to_string(it.second.dimming));
		weather.append(",");
		weather.append(it.second.weather);
		weather.append("\n");
		currentWeather.append(weather);
	}
	return currentWeather;
}

std::string CharacterCoreWriter::writeMerchandStates(const CharacterCoreData& data) const {
	string states = "# merchand states:\n";

	for (auto& it : data.merchantStates) {
		string merchant = string(MERCHANT_STATE);
		merchant.append(":");
		merchant.append(it.first);
		merchant.append(",");
		merchant.append(to_string((int)(it.second.fraction)));
		merchant.append(",");
		merchant.append(to_string(it.second.multiplier));

		for (auto& it2 : it.second.wares) {
			merchant.append(",");
			merchant.append(it2.first);
			merchant.append(",");
			merchant.append(to_string(it2.second));
		}

		merchant.append("\n");
		states.append(merchant);
	}
	return states;
}

std::string CharacterCoreWriter::writeQuestProgressTargets(const CharacterCoreData& data) const {
	string quests = "# quest targets killed:\n";

	for (auto& it : data.questTargetProgress) {
		string quest = string(QUEST_PROGRESS_TARGET);
		quest.append(":");
		quest.append(it.first);
		for (auto& it2 : it.second) {
			quest.append(",");
			quest.append(it2.first);
			quest.append(",");
			quest.append(to_string(static_cast<int>(it2.second)));
		}
		quest.append("\n");
		quests.append(quest);
	}
	return quests;
}

std::string CharacterCoreWriter::writeQuestProgressConditions(const CharacterCoreData& data) const {
	string quests = "# quest conditions fulfilled:\n";

	for (auto& it : data.questConditionProgress) {
		if (it.second.empty()) continue;
		string quest = string(QUEST_PROGRESS_CONDITION);
		quest.append(":");
		quest.append(it.first);
		for (auto& it2 : it.second) {
			quest.append(",");
			quest.append(it2);
		}
		quest.append("\n");
		quests.append(quest);
	}
	return quests;
}

std::string CharacterCoreWriter::writeQuestProgressDescription(const CharacterCoreData& data) const {
	string quests = "# quest descriptions unlocked:\n";

	for (auto& it : data.questDescriptionProgress) {
		if (it.second.empty()) continue;
		string quest = string(QUEST_PROGRESS_DESCRIPTION);
		quest.append(":");
		quest.append(it.first);
		for (auto& it2 : it.second) {
			quest.append(",");
			quest.append(std::to_string(it2));
		}
		quest.append("\n");
		quests.append(quest);
	}
	return quests;
}

std::string CharacterCoreWriter::writeProgressConditions(const CharacterCoreData& data) const {
	string progress = "# conditions of a certain type fulfilled:\n";
	if (data.conditionProgress.empty()) {
		return progress;
	}

	for (auto& progressType : data.conditionProgress) {
		progress.append(string(PROGRESS_CONDITION));
		progress.append(":");
		progress.append(progressType.first);
		progress.append(",");

		for (auto& condition : progressType.second) {
			progress.append(condition);
			progress.append(",");
		}
		// remove last comma
		progress.pop_back();
		progress.append("\n");
	}

	return progress;
}

std::string CharacterCoreWriter::writeItemID(const CharacterCoreData& data) const {
	string items = "# items in inventory:\n";

	for (auto& it : data.items) {
		string item = string(ITEM_ID);
		item.append(":");
		item.append(it.first);
		item.append(",");
		item.append(to_string(it.second));
		item.append("\n");
		items.append(item);
	}
	return items;
}
std::string CharacterCoreWriter::writeEquippedWeaponSlots(const CharacterCoreData& data) const {
	string weaponSlots = "# weapon slots. it is always: spell ID+, (modifier type, level)*:\n";
	for (auto& it : data.equippedWeaponSlots) {
		weaponSlots.append(string(EQUIPPED_SPELLSLOT) + ":");
		weaponSlots.append(to_string(static_cast<int>(it.first)));
		for (auto& it2 : it.second) {
			weaponSlots.append(",");
			weaponSlots.append(to_string(static_cast<int>(it2.type)) + ",");
			weaponSlots.append(to_string(it2.level));
		}
		weaponSlots.append("\n");
	}
	return weaponSlots;
}

std::string CharacterCoreWriter::writeEquippedItems(const CharacterCoreData& data) const {
	string equipment = "# equipment:\n";

	equipment.append(string(EQUIPPED_BACK) + ":" + data.equippedItems.at(ItemType::Equipment_back) + "\n");
	equipment.append(string(EQUIPPED_BODY) + ":" + data.equippedItems.at(ItemType::Equipment_body) + "\n");
	equipment.append(string(EQUIPPED_HEAD) + ":" + data.equippedItems.at(ItemType::Equipment_head) + "\n");
	equipment.append(string(EQUIPPED_NECK) + ":" + data.equippedItems.at(ItemType::Equipment_neck) + "\n");
	equipment.append(string(EQUIPPED_RING_1) + ":" + data.equippedItems.at(ItemType::Equipment_ring_1) + "\n");
	equipment.append(string(EQUIPPED_RING_2) + ":" + data.equippedItems.at(ItemType::Equipment_ring_2) + "\n");
	equipment.append(string(EQUIPPED_WEAPON) + ":" + data.equippedItems.at(ItemType::Equipment_weapon) + "\n");

	return equipment;
}

std::string CharacterCoreWriter::writeQuickslots(const CharacterCoreData& data) const {
	string quickslots = "# quickslot assignments:\n";

	quickslots.append(string(QUICKSLOT) + ":1," + data.quickSlot1 + "\n");
	quickslots.append(string(QUICKSLOT) + ":2," + data.quickSlot2 + "\n");

	return quickslots;
}