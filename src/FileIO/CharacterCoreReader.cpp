#include "FileIO/CharacterCoreReader.h"
#include "Misc/CBit.h"

#include <cstring>

#define INT_AFTER_COMMA(s, i) if (!readIntAfterComma(s, &i)) return false;
#define FLOAT_AFTER_COMMA(s, f) if (!readFloatAfterComma(s, &f)) return false;

CharacterCoreReader::CharacterCoreReader() {
	initReadMap();
}

void CharacterCoreReader::initReadMap() {
	m_readMap.insert({ TIME_PLAYED,  &CharacterCoreReader::readTimePlayed });
	m_readMap.insert({ SAVE_GAME_NAME,  &CharacterCoreReader::readSavegameName });
	m_readMap.insert({ DATE_SAVED,  &CharacterCoreReader::readSavegameDate });
	m_readMap.insert({ ATTRIBUTES,  &CharacterCoreReader::readAttributes });
	m_readMap.insert({ MAP_ID,  &CharacterCoreReader::readMapID });
	m_readMap.insert({ MAP_POSITION,  &CharacterCoreReader::readMapPosition });
	m_readMap.insert({ FORCED_MAP_ID,  &CharacterCoreReader::readForcedMapID });
	m_readMap.insert({ FORCED_MAP_POSITION,  &CharacterCoreReader::readForcedMapPosition });
	m_readMap.insert({ LEVEL_ID,  &CharacterCoreReader::readLevelID });
	m_readMap.insert({ LEVEL_POSITION,  &CharacterCoreReader::readLevelPosition });
	m_readMap.insert({ ENEMIES_KILLED,  &CharacterCoreReader::readEnemiesKilled });
	m_readMap.insert({ ENEMIES_LOOTED,  &CharacterCoreReader::readEnemiesLooted });
	m_readMap.insert({ ITEMS_LOOTED,  &CharacterCoreReader::readItemsLooted });
	m_readMap.insert({ CHESTS_LOOTED,  &CharacterCoreReader::readChestsLooted });
	m_readMap.insert({ WAYPOINTS_UNLOCKED,  &CharacterCoreReader::readWaypointsUnlocked });
	m_readMap.insert({ TRIGGERS_TRIGGERED,  &CharacterCoreReader::readTriggersTriggered });
	m_readMap.insert({ TILES_EXPLORED,  &CharacterCoreReader::readTilesExplored });
	m_readMap.insert({ QUEST_STATE,  &CharacterCoreReader::readQuestStates });
	m_readMap.insert({ QUEST_PROGRESS_TARGET,  &CharacterCoreReader::readQuestProgressTargets });
	m_readMap.insert({ QUEST_PROGRESS_CONDITION,  &CharacterCoreReader::readQuestProgressConditions });
	m_readMap.insert({ QUEST_PROGRESS_DESCRIPTION,  &CharacterCoreReader::readQuestProgressDescription });
	m_readMap.insert({ PROGRESS_CONDITION,  &CharacterCoreReader::readProgressConditions });
	m_readMap.insert({ GOLD,  &CharacterCoreReader::readGold });
	m_readMap.insert({ STORED_GOLD,  &CharacterCoreReader::readStoredGold });
	m_readMap.insert({ ITEM_ID,  &CharacterCoreReader::readItemID });
	m_readMap.insert({ STORED_ITEM_ID,  &CharacterCoreReader::readStoredItemID });
	m_readMap.insert({ EQUIPPED_SPELLSLOT,  &CharacterCoreReader::readEquippedWeaponSlots });
	m_readMap.insert({ EQUIPPED_WEAPON,  &CharacterCoreReader::readEquippedWeapon });
	m_readMap.insert({ EQUIPPED_BODY,  &CharacterCoreReader::readEquippedBody });
	m_readMap.insert({ EQUIPPED_RING_1,  &CharacterCoreReader::readEquippedRing1 });
	m_readMap.insert({ EQUIPPED_RING_2,  &CharacterCoreReader::readEquippedRing2 });
	m_readMap.insert({ EQUIPPED_NECK,  &CharacterCoreReader::readEquippedNeck });
	m_readMap.insert({ EQUIPPED_BACK,  &CharacterCoreReader::readEquippedBack });
	m_readMap.insert({ EQUIPPED_HEAD,  &CharacterCoreReader::readEquippedHead });
	m_readMap.insert({ WEAPON_CONFIGS,  &CharacterCoreReader::readWeaponConfigurations });
	m_readMap.insert({ WEAPON_SPELL,  &CharacterCoreReader::readWeaponSpell });
	m_readMap.insert({ QUICKSLOT,  &CharacterCoreReader::readQuickslot });
	m_readMap.insert({ IS_IN_LEVEL,  &CharacterCoreReader::readIsInLevel });
	m_readMap.insert({ MERCHANT_STATE,  &CharacterCoreReader::readMerchantState });
	m_readMap.insert({ SPELL_LEARNED,  &CharacterCoreReader::readLearnedSpells });
	m_readMap.insert({ MODIFIER_LEARNED,  &CharacterCoreReader::readLearnedModifiers });
	m_readMap.insert({ HINT_LEARNED,  &CharacterCoreReader::readLearnedHints });
	m_readMap.insert({ GUILD,  &CharacterCoreReader::readGuild });
	m_readMap.insert({ REPUTATION_PROGRESS,  &CharacterCoreReader::readReputationProgress });
	m_readMap.insert({ WEATHER,  &CharacterCoreReader::readWeather });
}

bool CharacterCoreReader::readCharacterCore(const std::string& filename, CharacterCoreData& data, bool onlySaveGame) {
	std::string line;
	std::ifstream saveFile(filename);

	if (!saveFile.is_open()) {
		g_logger->logError("CharacterCoreReader", "Error at opening file " + filename);
		return false;
	}

	std::size_t colonPos;
	std::string tag;
	bool noError = true;

	while (!safeGetline(saveFile, line).eof()) {
		if (line.empty() || line.at(0) == COMMENT_MARKER) continue;

		colonPos = line.find(':');
		if (colonPos == std::string::npos) continue;

		tag = line.substr(0, colonPos);

		auto mapIt = m_readMap.find(tag);

		if (mapIt == m_readMap.end()) {
			g_logger->logError("CharacterCoreReader", "Unknown tag found in save file: " + tag);
			return false;
		}

		line = line.substr(colonPos + 1);
		if (line.empty()) {
			continue;
		}

		if (!(*this.*(mapIt->second))(line, data)) {
			noError = false;
			break;
		}

		if (onlySaveGame && m_stopReading && noError) {
			saveFile.close();
			return true;
		}
	}

	saveFile.close();

	// check data
	if (!noError || !checkData(data)) {
		return false;
	}

	return true;
}

std::istream& CharacterCoreReader::safeGetline(std::istream& is, std::string& t) {
	t.clear();

	std::istream::sentry se(is, true);
	std::streambuf* sb = is.rdbuf();

	for (;;) {
		int c = sb->sbumpc();
		switch (c) {
		case '\n':
			return is;
		case '\r':
			if (sb->sgetc() == '\n')
				sb->sbumpc();
			return is;
		case EOF:
			// Also handle the case when the last line has no line ending
			if (t.empty())
				is.setstate(std::ios::eofbit);
			return is;
		default:
			t += (char)c;
		}
	}
}

bool CharacterCoreReader::checkData(CharacterCoreData& data) const {
	if (data.currentMapPosition.x < 0 || data.currentMapPosition.y < 0) {
		g_logger->logError("CharacterCoreReader", "Error in savegame data : map position negative");
		return false;
	}
	if (data.currentMap.empty()) {
		g_logger->logError("CharacterCoreReader", "Error in savegame data : map not resolved");
		return false;
	}
	if (data.gold < 0) {
		g_logger->logError("CharacterCoreReader", "Error in savegame data : gold cannot be negative");
		return false;
	}
	if (data.attributes.maxHealthPoints < 0
		|| data.attributes.healthRegenerationPerS < 0
		|| data.attributes.haste < 0
		|| data.attributes.critical < 0
		|| data.attributes.damagePhysical < 0
		|| data.attributes.damageFire < 0
		|| data.attributes.damageIce < 0
		|| data.attributes.damageShadow < 0
		|| data.attributes.damageLight < 0) {
		g_logger->logError("CharacterCoreReader", "Error in savegame data : (some) attributes cannot be negative");
		return false;
	}
	if (data.equippedWeaponSlots.size() > 5) {
		g_logger->logError("CharacterCoreReader", "Error in savegame data : there can't be more spell slots than 5 on a weapon");
		return false;
	}
	for (auto& it : data.questStates) {
		if (it.first.empty()) {
			g_logger->logError("CharacterCoreReader", "Error in savegame data : quest id empty");
			return false;
		}
	}

	return true;
}

bool CharacterCoreReader::readTimePlayed(std::string& line, CharacterCoreData& data) const {
	int x;
	if (!readPositiveInt(line, &x)) return false;
	data.timePlayed = sf::seconds(static_cast<float>(x));
	return true;
}

bool CharacterCoreReader::readSavegameName(std::string& line, CharacterCoreData& data) const {
	data.saveGameName = line;
	return true;
}

bool CharacterCoreReader::readWeaponSpell(std::string& line, CharacterCoreData& data) const {
	Key x = static_cast<Key>(stoi(line));
	if (x <= Key::VOID || x > Key::MAX) {
		return false;
	}
	data.weaponSpell = x;
	return true;
}

bool CharacterCoreReader::readWeaponConfigurations(std::string& line, CharacterCoreData& data) const {
	std::size_t pos = line.find(';');
	if (pos == std::string::npos) return false;

	std::string itemID = line.substr(0, pos);
	std::vector<WeaponSlot> equippedWeaponSlots;

	line = line.substr(pos + 1);

	while (!line.empty()) {
		pos = line.find(';');

		if (pos == std::string::npos || line.size() == 1) break;

		SpellID spell = static_cast<SpellID>(stoi(line));
		if (spell < SpellID::VOID || spell >= SpellID::MAX) {
			g_logger->logError("CharacterCoreReader", "Spell ID not recognized: " + std::to_string(static_cast<int>(spell)));
			return false;
		}

		size_t startSpellData = line.find(',');
		size_t endSpellData = line.find(';');

		if (endSpellData == std::string::npos) return false;

		std::vector<SpellModifier> modifiers;

		while (startSpellData != std::string::npos && startSpellData < endSpellData) {
			line = line.substr(startSpellData + 1);
			SpellModifierType type = static_cast<SpellModifierType>(std::stoi(line));
			if (type < SpellModifierType::VOID || type >= SpellModifierType::MAX) {
				g_logger->logError("CharacterCoreReader", "Spell Modifier type not recognized: " + std::to_string(static_cast<int>(type)));
				return false;
			}

			startSpellData = line.find(',');
			line = line.substr(startSpellData + 1);
			int level = std::stoi(line);
			if (level < 1 || level > 3) {
				if (type == SpellModifierType::VOID) {
					level = 0;
				}
				else {
					g_logger->logError("CharacterCoreReader", "Spell Modifier level is not allowed: " + std::to_string(level));
					return false;
				}
			}

			SpellModifier modifier;
			modifier.type = type;
			modifier.level = level;
			modifiers.push_back(modifier);

			startSpellData = line.find(',');
			endSpellData = line.find(';');
		}

		line = line.substr(endSpellData + 1);
		equippedWeaponSlots.push_back(WeaponSlot(spell, modifiers));
	}

	data.weaponConfigurations[itemID] = equippedWeaponSlots;

	return true;
}

bool CharacterCoreReader::readQuickslot(std::string& line, CharacterCoreData& data) const {
	int nr = std::stoi(line);
	if (nr < 1 || nr > 2) {
		g_logger->logError("CharacterCoreReader::readQuickslot", "Quickslot nr not valid.");
		return false;
	}

	std::size_t pos = line.find(',');

	if (pos == std::string::npos) return false;
	line = line.substr(pos + 1);

	if (nr == 1) {
		data.quickSlot1 = line;
	}
	else {
		data.quickSlot2 = line;
	}
	return true;
}

bool CharacterCoreReader::readSavegameDate(std::string& line, CharacterCoreData& data) const {
	data.dateSaved = (std::stol(line));
	const_cast<bool&>(m_stopReading) = true;
	return true;
}

bool CharacterCoreReader::readGold(std::string& line, CharacterCoreData& data) const {
	return readPositiveInt(line, &data.gold);
}

bool CharacterCoreReader::readStoredGold(std::string& line, CharacterCoreData& data) const {
	return readPositiveInt(line, &data.storedGold);
}

bool CharacterCoreReader::readMapID(std::string& line, CharacterCoreData& data) const {
	data.currentMap = line;
	return true;
}

bool CharacterCoreReader::readMapPosition(std::string& line, CharacterCoreData& data) const {
	data.currentMapPosition.x = static_cast<float>(std::stof(line));
	FLOAT_AFTER_COMMA(line, data.currentMapPosition.y);
	return true;
}

bool CharacterCoreReader::readForcedMapID(std::string& line, CharacterCoreData& data) const {
	data.forcedMap = line;
	return true;
}

bool CharacterCoreReader::readForcedMapPosition(std::string& line, CharacterCoreData& data) const {
	data.forcedMapPosition.x = static_cast<float>(std::stof(line));
	FLOAT_AFTER_COMMA(line, data.forcedMapPosition.y);
	return true;
}

bool CharacterCoreReader::readLevelID(std::string& line, CharacterCoreData& data) const {
	data.currentLevel = line;
	return true;
}

bool CharacterCoreReader::readLevelPosition(std::string& line, CharacterCoreData& data) const {
	data.currentLevelPosition.x = static_cast<float>(std::stof(line));
	FLOAT_AFTER_COMMA(line, data.currentLevelPosition.y);
	return true;
}

bool CharacterCoreReader::readAttributes(std::string& line, CharacterCoreData& data) const {
	data.attributes.maxHealthPoints = std::stoi(line);
	INT_AFTER_COMMA(line, data.attributes.healthRegenerationPerS);
	INT_AFTER_COMMA(line, data.attributes.haste);
	INT_AFTER_COMMA(line, data.attributes.critical);
	INT_AFTER_COMMA(line, data.attributes.damagePhysical);
	INT_AFTER_COMMA(line, data.attributes.damageFire);
	INT_AFTER_COMMA(line, data.attributes.damageIce);
	INT_AFTER_COMMA(line, data.attributes.damageShadow);
	INT_AFTER_COMMA(line, data.attributes.damageLight);
	INT_AFTER_COMMA(line, data.attributes.resistancePhysical);
	INT_AFTER_COMMA(line, data.attributes.resistanceFire);
	INT_AFTER_COMMA(line, data.attributes.resistanceIce);
	INT_AFTER_COMMA(line, data.attributes.resistanceShadow);
	INT_AFTER_COMMA(line, data.attributes.resistanceLight);
	return true;
}

bool CharacterCoreReader::readItemID(std::string& line, CharacterCoreData& data) const {
	std::size_t pos = line.find(',');
	if (pos == std::string::npos || pos == 0) return false;

	std::string id = line.substr(0, pos);
	int amount = 0;
	INT_AFTER_COMMA(line, amount);
	if (amount <= 0) return false;

	data.items.insert({ id, amount });
	return true;
}

bool CharacterCoreReader::readStoredItemID(std::string& line, CharacterCoreData& data) const {
	std::size_t pos = line.find(',');
	if (pos == std::string::npos || pos == 0) return false;

	std::string id = line.substr(0, pos);
	int amount = 0;
	INT_AFTER_COMMA(line, amount);
	if (amount <= 0) return false;

	data.storedItems.insert({ id, amount });
	return true;
}

bool CharacterCoreReader::readIsInLevel(std::string& line, CharacterCoreData& data) const {
	data.isInLevel = std::stoi(line) == 1;
	return true;
}

bool CharacterCoreReader::readWeather(std::string& line, CharacterCoreData& data) const {
	std::size_t pos = line.find(',');
	if (pos == std::string::npos || pos == 0) return false;

	std::string id = line.substr(0, pos);
	float dimming = 0.f;

	FLOAT_AFTER_COMMA(line, dimming);
	if (dimming < 0.f || dimming > 1.f) {
		g_logger->logError("CharacterCoreReader", "Weather could not be read: Dimming has to be between 0.f and 1.f!");
		return false;
	}

	pos = line.find(',');
	if (pos == std::string::npos) return false;

	line = line.substr(pos + 1);

	WeatherData weatherData;
	weatherData.dimming = dimming;
	weatherData.weather = line;

	data.currentWeather.insert({ id, weatherData });
	return true;
}

bool CharacterCoreReader::readMerchantState(std::string& line, CharacterCoreData& data) const {
	std::size_t pos = line.find(',');
	if (pos == std::string::npos || pos == 0) return false;

	std::string id = line.substr(0, pos);
	std::map<std::string, int> wares;

	line = line.substr(pos + 1);

	pos = line.find(',');

	while (pos != std::string::npos) {
		std::string name = line.substr(0, pos);
		int amount;
		INT_AFTER_COMMA(line, amount);

		if (amount <= 0) return false;

		wares.insert({ name, amount });
		pos = line.find(',');

		if (pos == std::string::npos) {
			break;
		}

		line = line.substr(pos + 1);
		pos = line.find(',');
	}

	data.merchantStates.insert({ id, wares });
	return true;
}

bool CharacterCoreReader::readQuestStates(std::string& line, CharacterCoreData& data) const {
	std::size_t pos = line.find(',');
	if (pos == std::string::npos || pos == 0) return false;

	std::string questID = line.substr(0, pos);
	int questState;
	INT_AFTER_COMMA(line, questState);

	QuestState state = static_cast<QuestState>(questState);
	if (state <= QuestState::VOID || state >= QuestState::MAX) {
		g_logger->logError("CharacterCoreReader", "Quest State not recognized: " + std::to_string(questState));
		return false;
	}
	data.questStates.insert({ questID, state });
	return true;
}

bool CharacterCoreReader::readGuild(std::string& line, CharacterCoreData& data) const {
	FractionID id = resolveFractionID(line);
	if (id == FractionID::VOID) {
		return true;
	}
	data.guild = id;
	return true;
}

bool CharacterCoreReader::readReputationProgress(std::string& line, CharacterCoreData& data) const {
	std::size_t pos = line.find(',');
	if (pos == std::string::npos || pos == 0) return false;

	std::string fractionName = line.substr(0, pos);

	FractionID id = resolveFractionID(fractionName);
	if (id == FractionID::VOID) {
		g_logger->logWarning("CharacterCoreReader", "Fraction ID not recognized: " + fractionName);
		return true;
	}

	int reputation;
	INT_AFTER_COMMA(line, reputation);
	if (reputation < 0 || reputation > 100) {
		g_logger->logError("CharacterCoreReader", "Reputation for " + fractionName + " could not be read, it has to be between 0 and 100");
		return false;
	}

	data.reputationProgress.insert({ id, reputation });
	return true;
}

bool CharacterCoreReader::readQuestProgressConditions(std::string& line, CharacterCoreData& data) const {
	std::size_t pos = line.find(',');
	if (pos == std::string::npos || pos == 0) return false;

	std::string questID = line.substr(0, pos);
	line = line.substr(pos + 1);

	std::set<std::string> conditions;
	while (!line.empty()) {
		pos = line.find(',');

		if (pos == std::string::npos) {
			conditions.insert(line);
			break;
		}

		conditions.insert(line.substr(0, pos));
		line = line.substr(pos + 1);
	}

	data.questConditionProgress.insert({ questID, conditions });
	return true;
}

bool CharacterCoreReader::readQuestProgressDescription(std::string& line, CharacterCoreData& data) const {
	std::size_t pos = line.find(',');
	if (pos == std::string::npos || pos == 0) return false;

	std::string questID = line.substr(0, pos);
	line = line.substr(pos + 1);

	std::set<int> descriptions;
	while (!line.empty()) {

		int desc = std::stoi(line);
		if (desc < 1) {
			g_logger->logError("CharacterCoreReader", "Cannot insert quest description id:" + std::to_string(desc));
			return false;
		}

		descriptions.insert(desc);

		pos = line.find(',');
		if (pos == std::string::npos) {
			break;
		}
		line = line.substr(pos + 1);
	}

	data.questDescriptionProgress.insert({ questID, descriptions });
	return true;
}

bool CharacterCoreReader::readProgressConditions(std::string& line, CharacterCoreData& data) const {
	std::size_t pos = line.find(',');
	if (pos == std::string::npos || pos == 0) return false;

	std::string progressType = line.substr(0, pos);
	line = line.substr(pos + 1);

	if (contains(data.conditionProgress, progressType)) {
		g_logger->logError("CharacterCoreReader", "Duplicate condition progress type encountered: " + progressType);
		return false;
	}

	std::set<std::string> conditions;
	while (!line.empty()) {
		pos = line.find(',');

		if (pos == std::string::npos) {
			conditions.insert(line);
			break;
		}

		conditions.insert(line.substr(0, pos));
		line = line.substr(pos + 1);
	}

	data.conditionProgress.insert({ progressType, conditions });
	return true;
}

bool CharacterCoreReader::readQuestProgressTargets(std::string& line, CharacterCoreData& data) const {
	std::size_t pos = line.find(',');
	if (pos == std::string::npos || pos == 0) return false;

	std::string questID = line.substr(0, pos);
	line = line.substr(pos + 1);

	pos = line.find(',');

	std::map<std::string, int> targets;
	while (pos != std::string::npos) {
		std::string name = line.substr(0, pos);
		int amount;

		INT_AFTER_COMMA(line, amount);

		if (amount <= 0) {
			return false;
		}

		targets.insert({ name, amount });

		pos = line.find(',');

		if (pos == std::string::npos) {
			break;
		}

		line = line.substr(pos + 1);
		pos = line.find(',');
	}

	data.questTargetProgress.insert({ questID, targets });
	return true;
}

bool CharacterCoreReader::readLearnedSpells(std::string& line, CharacterCoreData& data) const {
	SpellType type = static_cast<SpellType>(std::stoi(line));
	if (type <= SpellType::VOID || type >= SpellType::MAX) {
		g_logger->logError("CharacterCoreReader", "Spell type not recognized: " + std::to_string(static_cast<int>(type)));
		return false;
	}

	std::set<SpellID> learnedSpells;

	std::size_t pos = line.find(',');

	while (pos != std::string::npos) {
		line = line.substr(pos + 1);

		SpellID id = static_cast<SpellID>(std::stoi(line));
		if (id <= SpellID::VOID || id >= SpellID::MAX) {
			g_logger->logError("CharacterCoreReader", "Spell id not recognized: " + std::to_string(static_cast<int>(id)));
			return false;
		}
		learnedSpells.insert(id);
		pos = line.find(',');
	}

	data.spellsLearned.insert({ type, learnedSpells });
	return true;
}

bool CharacterCoreReader::readLearnedModifiers(std::string& line, CharacterCoreData& data) const {
	SpellModifierType type = static_cast<SpellModifierType>(std::stoi(line));
	if (type <= SpellModifierType::VOID || type >= SpellModifierType::MAX) {
		g_logger->logError("CharacterCoreReader", "Spell modifier type not recognized: " + std::to_string(static_cast<int>(type)));
		return false;
	}

	int level;
	INT_AFTER_COMMA(line, level);

	if (level < 1 || level > 3) {
		g_logger->logError("CharacterCoreReader", "Spell modifier type level is not between 1 and 3");
		return false;
	}

	data.modfiersLearned.insert({ type, level });
	return true;
}

bool CharacterCoreReader::readLearnedHints(std::string& line, CharacterCoreData& data) const {
	data.hintsLearned.push_back(line);
	return true;
}

bool CharacterCoreReader::readEquippedWeapon(std::string& line, CharacterCoreData& data) const {
	data.equippedItems.at(ItemType::Equipment_weapon) = line;
	return true;
}

bool CharacterCoreReader::readEquippedHead(std::string& line, CharacterCoreData& data) const {
	data.equippedItems.at(ItemType::Equipment_head) = line;
	return true;
}

bool CharacterCoreReader::readEquippedNeck(std::string& line, CharacterCoreData& data) const {
	data.equippedItems.at(ItemType::Equipment_neck) = line;
	return true;
}

bool CharacterCoreReader::readEquippedBody(std::string& line, CharacterCoreData& data) const {
	data.equippedItems.at(ItemType::Equipment_body) = line;
	return true;
}

bool CharacterCoreReader::readEquippedBack(std::string& line, CharacterCoreData& data) const {
	data.equippedItems.at(ItemType::Equipment_back) = line;
	return true;
}

bool CharacterCoreReader::readEquippedRing1(std::string& line, CharacterCoreData& data) const {
	data.equippedItems.at(ItemType::Equipment_ring_1) = line;
	return true;
}

bool CharacterCoreReader::readEquippedRing2(std::string& line, CharacterCoreData& data) const {
	data.equippedItems.at(ItemType::Equipment_ring_2) = line;
	return true;
}

bool CharacterCoreReader::readEquippedWeaponSlots(std::string& line, CharacterCoreData& data) const {
	SpellID spell = static_cast<SpellID>(std::stoi(line));
	if (spell < SpellID::VOID || spell >= SpellID::MAX) {
		g_logger->logError("CharacterCoreReader", "Spell ID not recognized: " + std::to_string(static_cast<int>(spell)));
		return false;
	}

	std::vector<SpellModifier> modifiers;
	std::size_t pos = line.find(',');

	while (pos != std::string::npos) {
		line = line.substr(pos + 1);
		SpellModifierType type = static_cast<SpellModifierType>(std::stoi(line));
		if (type < SpellModifierType::VOID || type >= SpellModifierType::MAX) {
			g_logger->logError("CharacterCoreReader", "Spell Modifier type not recognized: " + std::to_string(static_cast<int>(type)));
			return false;
		}

		int level;
		INT_AFTER_COMMA(line, level);
		if (level < 1 || level > 3) {
			if (type == SpellModifierType::VOID) {
				level = 0;
			}
			else {
				g_logger->logError("CharacterCoreReader", "Spell Modifier level is not allowed: " + std::to_string(level));
				return false;
			}
		}

		SpellModifier modifier;
		modifier.type = type;
		modifier.level = level;
		modifiers.push_back(modifier);

		pos = line.find(',');
	}

	data.equippedWeaponSlots.push_back(std::pair<SpellID, std::vector<SpellModifier>>(spell, modifiers));

	return true;
}

bool CharacterCoreReader::readLevelStateLayer(std::string& line, std::set<int>& layer, std::string& id) const {
	std::size_t pos = line.find(',');
	if (pos == std::string::npos) {
		id = line;
		layer.clear();
		return true;
	}

	id = line.substr(0, pos);

	while (pos != std::string::npos) {
		line = line.substr(pos + 1);
		layer.insert(std::stoi(line));
		pos = line.find(',');
	}

	return true;
}

bool CharacterCoreReader::readEnemiesKilled(std::string& line, CharacterCoreData& data) const {
	std::set<int> layer;
	std::string id;
	if (!readLevelStateLayer(line, layer, id)) return false;
	data.enemiesKilled.insert({ id, layer });
	return true;
}

bool CharacterCoreReader::readEnemiesLooted(std::string& line, CharacterCoreData& data) const {
	std::set<int> layer;
	std::string id;
	if (!readLevelStateLayer(line, layer, id)) return false;
	data.enemiesLooted.insert({ id, layer });
	return true;
}

bool CharacterCoreReader::readItemsLooted(std::string& line, CharacterCoreData& data) const {
	std::set<int> layer;
	std::string id;
	if (!readLevelStateLayer(line, layer, id)) return false;
	data.itemsLooted.insert({ id, layer });
	return true;
}

bool CharacterCoreReader::readChestsLooted(std::string& line, CharacterCoreData& data) const {
	std::set<int> layer;
	std::string id;
	if (!readLevelStateLayer(line, layer, id)) return false;
	data.chestsLooted.insert({ id, layer });
	return true;
}

bool CharacterCoreReader::readWaypointsUnlocked(std::string& line, CharacterCoreData& data) const {
	std::map<int, sf::Vector2f> layer;
	std::string id;

	std::size_t pos = line.find(',');
	if (pos == std::string::npos) {
		id = line;
		layer.clear();
		return true;
	}

	id = line.substr(0, pos);

	while (pos != std::string::npos) {
		line = line.substr(pos + 1);
		sf::Vector2f v;
		int id;
		id = std::stoi(line);
		pos = line.find(',');
		if (pos == std::string::npos) return false;
		line = line.substr(pos + 1);
		v.x = std::stof(line);
		pos = line.find(',');
		if (pos == std::string::npos) return false;
		line = line.substr(pos + 1);
		v.y = std::stof(line);
		pos = line.find(',');
		layer[id] = v;
	}

	data.waypointsUnlocked.insert({ id, layer });
	return true;
}

bool CharacterCoreReader::readTriggersTriggered(std::string& line, CharacterCoreData& data) const {
	std::set<int> layer;
	std::string id;
	if (!readLevelStateLayer(line, layer, id)) return false;
	data.triggersTriggered.insert({ id, layer });
	return true;
}

bool CharacterCoreReader::readTilesExplored(std::string& line, CharacterCoreData& data) const {
	std::vector<bool> tiles;
	sf::Vector2i mapSize;

	std::size_t pos = line.find(',');
	if (pos == std::string::npos || pos == 0) return false;

	std::string levelID = line.substr(0, pos);

	INT_AFTER_COMMA(line, mapSize.x);
	INT_AFTER_COMMA(line, mapSize.y);

	pos = line.find(',');

	if (pos == std::string::npos) return false;
	line = line.substr(pos + 1);

	pos = line.find(',');

	if (pos == std::string::npos) return false;

	CBits byte;

	for (int i = 0; i < static_cast<int>(pos); ++i) {
		byte.byte = line.at(i);

		tiles.push_back(byte.bits.b0);
		tiles.push_back(byte.bits.b1);
		tiles.push_back(byte.bits.b2);
		tiles.push_back(byte.bits.b3);
		tiles.push_back(byte.bits.b4);
		tiles.push_back(byte.bits.b5);
		tiles.push_back(byte.bits.b6);
		tiles.push_back(byte.bits.b7);
	}

	line = line.substr(pos + 1);

	while (!line.empty()) {
		tiles.push_back(std::atoi(line.c_str()) == 1);
		line.erase(0, 1);
	}

	data.tilesExplored.insert({ levelID, {mapSize, tiles} });
	return true;
}
