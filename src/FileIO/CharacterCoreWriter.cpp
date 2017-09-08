#include "FileIO/CharacterCoreWriter.h"
#include "Enums/EnumNames.h"
#include "Misc/CBit.h"

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
	std::ofstream savefile(filename, std::ios::trunc);
	if (savefile.is_open()) {
		savefile << writeTimePlayed(data);
		savefile << writeSaveGameName(data);
		savefile << writeDateSaved(data);
		savefile << writeLevelID(data);
		savefile << writeLevelPosition(data);
		savefile << writeIsInLevel(data);
		savefile << writeMapID(data);
		savefile << writeMapPosition(data);
		savefile << writeForcedMapID(data);
		savefile << writeForcedMapPosition(data);
		savefile << writeAttributes(data);
		savefile << writeGold(data);
		savefile << writeItemID(data);
		savefile << writeStoredGold(data);
		savefile << writeStoredItemID(data);
		savefile << writeEquippedWeaponSlots(data);
		savefile << writeEquippedItems(data);
		savefile << writeWeaponConfigurations(data);
		savefile << writeWeaponSpell(data);
		savefile << writeQuickslots(data);
		savefile << writeEnemiesKilled(data);
		savefile << writeEnemiesLooted(data);
		savefile << writeItemsLooted(data);
		savefile << writeChestsLooted(data);
		savefile << writeWaypointsUnlocked(data);
		savefile << writeModifiersUnlocked(data);
		savefile << writeTriggersTriggered(data);
		savefile << writeTilesExplored(data);
		savefile << writeQuestStates(data);
		savefile << writeQuestProgressTargets(data);
		savefile << writeQuestProgressConditions(data);
		savefile << writeQuestProgressDescription(data);
		savefile << writeProgressConditions(data);
		savefile << writeGuild(data);
		savefile << writeReputationProgress(data);
		savefile << writeMerchantStates(data);
		savefile << writeSpellsLearned(data);
		savefile << writeModifiersLearned(data);
		savefile << writeHintsLearned(data);
		savefile << writeWeather(data);
		savefile << writeDeaths(data);

		savefile.close();
	}
	else {
		g_logger->logError("CharacterCoreWriter", "Unable to open file: " + std::string(filename));
		return false;
	}
	return true;
}

std::string CharacterCoreWriter::writeTimePlayed(const CharacterCoreData& data) const {
	std::string timePlayed = "# time played, in seconds\n";
	return timePlayed.append(std::string(TIME_PLAYED) + ":" + std::to_string(static_cast<int>(std::floor(data.timePlayed.asSeconds()))) + "\n");
}

std::string CharacterCoreWriter::writeSaveGameName(const CharacterCoreData& data) const {
	return std::string(SAVE_GAME_NAME) + ":" + data.saveGameName + "\n";
}

std::string CharacterCoreWriter::writeDateSaved(const CharacterCoreData& data) const {
	return std::string(DATE_SAVED) + ":" + std::to_string(data.dateSaved) + "\n";
}

std::string CharacterCoreWriter::writeMapID(const CharacterCoreData& data) const {
	return std::string(MAP_ID) + ":" + data.currentMap + "\n";
}

std::string CharacterCoreWriter::writeMapPosition(const CharacterCoreData& data) const {
	return std::string(MAP_POSITION) + ":" + std::to_string(data.currentMapPosition.x) + "," + std::to_string(data.currentMapPosition.y) + "\n";
}

std::string CharacterCoreWriter::writeForcedMapID(const CharacterCoreData& data) const {
	return std::string(FORCED_MAP_ID) + ":" + data.forcedMap + "\n";
}

std::string CharacterCoreWriter::writeForcedMapPosition(const CharacterCoreData& data) const {
	return std::string(FORCED_MAP_POSITION) + ":" + std::to_string(data.forcedMapPosition.x) + "," + std::to_string(data.forcedMapPosition.y) + "\n";
}

std::string CharacterCoreWriter::writeIsInLevel(const CharacterCoreData& data) const {
	return std::string(IS_IN_LEVEL) + ":" + (data.isInLevel ? "1" : "0") + "\n";
}

std::string CharacterCoreWriter::writeLevelID(const CharacterCoreData& data) const {
	return std::string(LEVEL_ID) + ":" + data.currentLevel + "\n";
}

std::string CharacterCoreWriter::writeLevelPosition(const CharacterCoreData& data) const {
	return std::string(LEVEL_POSITION) + ":" + std::to_string(data.currentLevelPosition.x) + "," + std::to_string(data.currentLevelPosition.y) + "\n";
}

std::string CharacterCoreWriter::writeEnemiesKilled(const CharacterCoreData& data) const {
	std::string enemiesKilled = "# enemies killed in level:\n";

	for (auto& it : data.enemiesKilled) {
		std::string enemyKilled = std::string(ENEMIES_KILLED);
		enemyKilled.append(":");
		enemyKilled.append(it.first);
		for (auto it2 : it.second) {
			enemyKilled.append("," + std::to_string(it2));
		}
		enemyKilled.append("\n");
		enemiesKilled.append(enemyKilled);
	}
	return enemiesKilled;
}

std::string CharacterCoreWriter::writeEnemiesLooted(const CharacterCoreData& data) const {
	std::string enemiesLooted = "# enemies looted in level:\n";

	for (auto& it : data.enemiesLooted) {
		std::string enemyLooted = std::string(ENEMIES_LOOTED);
		enemyLooted.append(":");
		enemyLooted.append(it.first);
		for (auto it2 : it.second) {
			enemyLooted.append("," + std::to_string(it2));
		}
		enemyLooted.append("\n");
		enemiesLooted.append(enemyLooted);
	}
	return enemiesLooted;
}

std::string CharacterCoreWriter::writeItemsLooted(const CharacterCoreData& data) const {
	std::string itemsLooted = "# items looted in level:\n";

	for (auto& it : data.itemsLooted) {
		std::string itemLooted = std::string(ITEMS_LOOTED);
		itemLooted.append(":");
		itemLooted.append(it.first);
		for (auto it2 : it.second) {
			itemLooted.append("," + std::to_string(it2));
		}
		itemLooted.append("\n");
		itemsLooted.append(itemLooted);
	}
	return itemsLooted;
}

std::string CharacterCoreWriter::writeWaypointsUnlocked(const CharacterCoreData& data) const {
	std::string waypointsUnlocked = "# waypoints unlocked in map:\n";

	for (auto& it : data.waypointsUnlocked) {
		std::string waypointUnlocked = std::string(WAYPOINTS_UNLOCKED);
		waypointUnlocked.append(":");
		waypointUnlocked.append(it.first);
		for (auto it2 : it.second) {
			waypointUnlocked.append("," + std::to_string(it2.first));
			waypointUnlocked.append("," + std::to_string(it2.second.x));
			waypointUnlocked.append("," + std::to_string(it2.second.y));
		}    
		waypointUnlocked.append("\n");
		waypointsUnlocked.append(waypointUnlocked);
	}

	return waypointsUnlocked;
}

std::string CharacterCoreWriter::writeTriggersTriggered(const CharacterCoreData& data) const {
	std::string triggersTriggered = "# triggers triggered in world:\n";

	for (auto& it : data.triggersTriggered) {
		std::string triggerTriggered = std::string(TRIGGERS_TRIGGERED);
		triggerTriggered.append(":");
		triggerTriggered.append(it.first);
		for (auto it2 : it.second) {
			triggerTriggered.append("," + std::to_string(it2));
		}
		triggerTriggered.append("\n");
		triggersTriggered.append(triggerTriggered);
	}
	return triggersTriggered;
}

std::string CharacterCoreWriter::writeTilesExplored(const CharacterCoreData& data) const {
	std::string tilesExplored = "# tiles explored in world:\n";

	for (auto& it : data.tilesExplored) {
		std::string tiles = std::string(TILES_EXPLORED);
		tiles.append(":");
		tiles.append(it.first);
		tiles.append(",");
		tiles.append(std::to_string(it.second.first.x));
		tiles.append(",");
		tiles.append(std::to_string(it.second.first.y));
		tiles.append(",");
		CBits byte;
		int iterations = static_cast<int>(std::floor(it.second.second.size() / 8.f) * 8);
		int i = 0;

		for (; i < iterations; i += 8) {
			byte.bits.b0 = it.second.second[i + 0];
			byte.bits.b1 = it.second.second[i + 1];
			byte.bits.b2 = it.second.second[i + 2];
			byte.bits.b3 = it.second.second[i + 3];
			byte.bits.b4 = it.second.second[i + 4];
			byte.bits.b5 = it.second.second[i + 5];
			byte.bits.b6 = it.second.second[i + 6];
			byte.bits.b7 = it.second.second[i + 7];
			tiles += byte.byte;
		}

		tiles.append(",");

		for (; i < it.second.second.size(); ++i) {
			tiles.append(std::to_string(it.second.second[i]));
		}

		tiles.append("\n");
		tilesExplored.append(tiles);
	}
	return tilesExplored;
}

std::string CharacterCoreWriter::writeChestsLooted(const CharacterCoreData& data) const {
	std::string chestsLooted = "# chests looted in level:\n";

	for (auto& it : data.chestsLooted) {
		std::string chestLooted = std::string(CHESTS_LOOTED);
		chestLooted.append(":");
		chestLooted.append(it.first);
		for (auto it2 : it.second) {
			chestLooted.append("," + std::to_string(it2));
		}
		chestLooted.append("\n");
		chestsLooted.append(chestLooted);
	}
	return chestsLooted;
}

std::string CharacterCoreWriter::writeSpellsLearned(const CharacterCoreData& data) const {
	std::string spellsLearned = "# spells learned:\n";

	for (auto& it : data.spellsLearned) {
		std::string spellLearned = std::string(SPELL_LEARNED);
		spellLearned.append(":");
		spellLearned.append(std::to_string(static_cast<int>(it.first)));
		for (auto it2 : it.second) {
			spellLearned.append("," + std::to_string(static_cast<int>(it2)));
		}
		spellLearned.append("\n");
		spellsLearned.append(spellLearned);
	}
	return spellsLearned;
}

std::string CharacterCoreWriter::writeModifiersLearned(const CharacterCoreData& data) const {
	std::string modifiersLearned = "# modifiers learned:\n";

	for (auto& it : data.modfiersLearned) {
		std::string modifierLearned = std::string(MODIFIER_LEARNED);
		modifierLearned.append(":");
		modifierLearned.append(std::to_string(static_cast<int>(it.first)));
		modifierLearned.append(",");
		modifierLearned.append(std::to_string(it.second));
		modifierLearned.append("\n");
		modifiersLearned.append(modifierLearned);
	}

	return modifiersLearned;
}

std::string CharacterCoreWriter::writeModifiersUnlocked(const CharacterCoreData& data) const {
	std::string modifiersUnlocked = "# modifiers unlocked:\n";

	for (auto& it : data.modifiersUnlocked) {
		std::string modifierUnlocked = std::string(MODIFIERS_UNLOCKED);
		modifierUnlocked.append(":");
		modifierUnlocked.append(it.first);
		for (auto it2 : it.second) {
			modifierUnlocked.append("," + std::to_string(it2));
		}
		modifierUnlocked.append("\n");
		modifiersUnlocked.append(modifierUnlocked);
	}

	return modifiersUnlocked;
}

std::string CharacterCoreWriter::writeAttributes(const CharacterCoreData& data) const {
	std::string orderComment = "# in this order: maxHealthPoints, healthRegenerationPerS, haste, critical, \n# damagePhysical, damageFire, damageIce, damageShadow, damageLight, \n# resistancePhysical, resistanceFire, resistanceIce, resistanceShadow, resistanceIce. \n";
	std::string attributeString = orderComment.append(std::string(ATTRIBUTES));
	attributeString.append(":");
	attributeString.append(std::to_string(data.attributes.maxHealthPoints) + ",");
	attributeString.append(std::to_string(data.attributes.healthRegenerationPerS) + ",");
	attributeString.append(std::to_string(data.attributes.haste) + ",");
	attributeString.append(std::to_string(data.attributes.critical) + ",");

	attributeString.append(std::to_string(data.attributes.damagePhysical) + ",");
	attributeString.append(std::to_string(data.attributes.damageFire) + ",");
	attributeString.append(std::to_string(data.attributes.damageIce) + ",");
	attributeString.append(std::to_string(data.attributes.damageShadow) + ",");
	attributeString.append(std::to_string(data.attributes.damageLight) + ",");

	attributeString.append(std::to_string(data.attributes.resistancePhysical) + ",");
	attributeString.append(std::to_string(data.attributes.resistanceFire) + ",");
	attributeString.append(std::to_string(data.attributes.resistanceIce) + ",");
	attributeString.append(std::to_string(data.attributes.resistanceShadow) + ",");
	attributeString.append(std::to_string(data.attributes.resistanceLight));
	attributeString.append("\n");

	return attributeString;
}

std::string CharacterCoreWriter::writeGold(const CharacterCoreData& data) const {
	return std::string(GOLD) + ":" + std::to_string(data.gold) + "\n";
}

std::string CharacterCoreWriter::writeDeaths(const CharacterCoreData& data) const {
	return std::string(DEATH_COUNT + ":" + std::to_string(data.deaths) + "\n";
}

std::string CharacterCoreWriter::writeStoredGold(const CharacterCoreData& data) const {
	return std::string(STORED_GOLD) + ":" + std::to_string(data.storedGold) + "\n";
}

std::string CharacterCoreWriter::writeQuestStates(const CharacterCoreData& data) const {
	std::string quests = "# quest states:\n";

	for (auto& it : data.questStates) {
		std::string quest = std::string(QUEST_STATE);
		quest.append(":");
		quest.append(it.first);
		quest.append(",");
		quest.append(std::to_string(static_cast<int>(it.second)));
		quest.append("\n");
		quests.append(quest);
	}
	return quests;
}

std::string CharacterCoreWriter::writeHintsLearned(const CharacterCoreData& data) const {
	std::string hintList = "# hints learned:\n";

	for (auto& it : data.hintsLearned) {
		std::string hint = std::string(HINT_LEARNED);
		hint.append(":");
		hint.append(it);
		hint.append("\n");
		hintList.append(hint);
	}

	return hintList;
}

std::string CharacterCoreWriter::writeGuild(const CharacterCoreData& data) const {
	std::string guild = "# guild:\n";

	guild.append(std::string(GUILD));
	guild.append(":");
	guild.append(EnumNames::getFractionIDName(data.guild));
	guild.append("\n");

	return guild;
}

std::string CharacterCoreWriter::writeReputationProgress(const CharacterCoreData& data) const {
	std::string reputationProgress = "# reputation progress:\n";

	for (auto& it : data.reputationProgress) {
		std::string reputation = std::string(REPUTATION_PROGRESS);
		reputation.append(":");
		reputation.append(EnumNames::getFractionIDName(it.first));
		reputation.append(",");
		reputation.append(std::to_string(it.second));
		reputation.append("\n");
		reputationProgress.append(reputation);
	}
	return reputationProgress;
}

std::string CharacterCoreWriter::writeWeather(const CharacterCoreData& data) const {
	std::string currentWeather = "# current weather:\n";

	for (auto& it : data.currentWeather) {
		std::string weather = std::string(WEATHER);
		weather.append(":");
		weather.append(it.first);
		weather.append(",");
		weather.append(std::to_string(it.second.ambientDimming));
		weather.append(",");
		weather.append(it.second.weather);
		weather.append("\n");
		currentWeather.append(weather);
	}
	return currentWeather;
}

std::string CharacterCoreWriter::writeMerchantStates(const CharacterCoreData& data) const {
	std::string states = "# merchant states:\n";

	for (auto& it : data.merchantStates) {
		std::string merchant = std::string(MERCHANT_STATE);
		merchant.append(":");
		merchant.append(it.first);

		for (auto& it2 : it.second) {
			merchant.append(",");
			merchant.append(it2.first);
			merchant.append(",");
			merchant.append(std::to_string(it2.second));
		}

		merchant.append("\n");
		states.append(merchant);
	}
	return states;
}

std::string CharacterCoreWriter::writeQuestProgressTargets(const CharacterCoreData& data) const {
	std::string quests = "# quest targets killed:\n";

	for (auto& it : data.questTargetProgress) {
		std::string quest = std::string(QUEST_PROGRESS_TARGET);
		quest.append(":");
		quest.append(it.first);
		for (auto& it2 : it.second) {
			quest.append(",");
			quest.append(it2.first);
			quest.append(",");
			quest.append(std::to_string(static_cast<int>(it2.second)));
		}
		quest.append("\n");
		quests.append(quest);
	}
	return quests;
}

std::string CharacterCoreWriter::writeQuestProgressConditions(const CharacterCoreData& data) const {
	std::string quests = "# quest conditions fulfilled:\n";

	for (auto& it : data.questConditionProgress) {
		if (it.second.empty()) continue;
		std::string quest = std::string(QUEST_PROGRESS_CONDITION);
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
	std::string quests = "# quest descriptions unlocked:\n";

	for (auto& it : data.questDescriptionProgress) {
		if (it.second.empty()) continue;
		std::string quest = std::string(QUEST_PROGRESS_DESCRIPTION);
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
	std::string progress = "# conditions of a certain type fulfilled:\n";
	if (data.conditionProgress.empty()) {
		return progress;
	}

	for (auto& progressType : data.conditionProgress) {
		if (progressType.second.empty()) continue;

		progress.append(std::string(PROGRESS_CONDITION));
		progress.append(":");
		progress.append(progressType.first);
		
		for (auto& condition : progressType.second) {
			progress.append(",");
			progress.append(condition);
		}

		progress.append("\n");
	}

	return progress;
}

std::string CharacterCoreWriter::writeItemID(const CharacterCoreData& data) const {
	std::string items = "# items in inventory:\n";

	for (auto& it : data.items) {
		std::string item = std::string(ITEM_ID);
		item.append(":");
		item.append(it.first);
		item.append(",");
		item.append(std::to_string(it.second));
		item.append("\n");
		items.append(item);
	}
	return items;
}

std::string CharacterCoreWriter::writeStoredItemID(const CharacterCoreData& data) const {
	std::string items = "# stored items:\n";

	for (auto& it : data.storedItems) {
		std::string item = std::string(STORED_ITEM_ID);
		item.append(":");
		item.append(it.first);
		item.append(",");
		item.append(std::to_string(it.second));
		item.append("\n");
		items.append(item);
	}
	return items;
}

std::string CharacterCoreWriter::writeEquippedWeaponSlots(const CharacterCoreData& data) const {
	std::string weaponSlots = "# weapon slots. it is always: spell ID+, (modifier type, level)*:\n";
	for (auto& it : data.equippedWeaponSlots) {
		weaponSlots.append(std::string(EQUIPPED_SPELLSLOT) + ":");
		weaponSlots.append(std::to_string(static_cast<int>(it.first)));
		for (auto& it2 : it.second) {
			weaponSlots.append(",");
			weaponSlots.append(std::to_string(static_cast<int>(it2.type)) + ",");
			weaponSlots.append(std::to_string(it2.level));
		}
		weaponSlots.append("\n");
	}
	return weaponSlots;
}

std::string CharacterCoreWriter::writeEquippedItems(const CharacterCoreData& data) const {
	std::string equipment = "# equipment:\n";

	equipment.append(std::string(EQUIPPED_BACK) + ":" + data.equippedItems.at(ItemType::Equipment_back) + "\n");
	equipment.append(std::string(EQUIPPED_BODY) + ":" + data.equippedItems.at(ItemType::Equipment_body) + "\n");
	equipment.append(std::string(EQUIPPED_HEAD) + ":" + data.equippedItems.at(ItemType::Equipment_head) + "\n");
	equipment.append(std::string(EQUIPPED_NECK) + ":" + data.equippedItems.at(ItemType::Equipment_neck) + "\n");
	equipment.append(std::string(EQUIPPED_RING_1) + ":" + data.equippedItems.at(ItemType::Equipment_ring_1) + "\n");
	equipment.append(std::string(EQUIPPED_RING_2) + ":" + data.equippedItems.at(ItemType::Equipment_ring_2) + "\n");
	equipment.append(std::string(EQUIPPED_WEAPON) + ":" + data.equippedItems.at(ItemType::Equipment_weapon) + "\n");

	return equipment;
}

std::string CharacterCoreWriter::writeWeaponSpell(const CharacterCoreData& data) const {
	return std::string(WEAPON_SPELL) + ":" + std::to_string(static_cast<int>(data.weaponSpell)) + "\n";
}

std::string CharacterCoreWriter::writeWeaponConfigurations(const CharacterCoreData& data) const {
	std::string weaponConfigs = "# weapon configurations:\n";

	for (auto& it : data.weaponConfigurations) {
		weaponConfigs.append(std::string(WEAPON_CONFIGS) + ":");
		weaponConfigs.append(it.first);
		weaponConfigs.append(";");

		for (auto it2 = it.second.begin(); it2 != it.second.end(); ++it2) {
			weaponConfigs.append(std::to_string(static_cast<int>(it2->first)));
			for (auto& it3 : it2->second) {
				weaponConfigs.append(",");
				weaponConfigs.append(std::to_string(static_cast<int>(it3.type)) + ",");
				weaponConfigs.append(std::to_string(it3.level));
			}
			weaponConfigs.append(";");
		}
		weaponConfigs.append("\n");
	}

	return weaponConfigs;
}

std::string CharacterCoreWriter::writeQuickslots(const CharacterCoreData& data) const {
	std::string quickslots = "# quickslot assignments:\n";

	quickslots.append(std::string(QUICKSLOT) + ":1," + data.quickSlot1 + "\n");
	quickslots.append(std::string(QUICKSLOT) + ":2," + data.quickSlot2 + "\n");

	return quickslots;
}