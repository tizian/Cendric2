#include "CharacterCore.h"
#include "World/Item.h"
#include "FileIO/MerchantLoader.h"
#include "Beans/SpawnBean.h"
#include "DatabaseManager.h"
#include "GlobalResource.h"
#include "Steam/AchievementManager.h"
#include "FileIO/CharacterCoreReader.h"
#include "FileIO/CharacterCoreWriter.h"

CharacterCore::CharacterCore() {
	for (ItemType type = ItemType::Equipment_head; type <= ItemType::Equipment_back; type = static_cast<ItemType>((int)type + 1)) {
		m_data.equippedItems.insert({ type, "" });
	}
}

CharacterCore::CharacterCore(const CharacterCoreData& data) {
	m_data = data;
	m_stopwatch.restart();
	loadWeapon();
	reloadAttributes();
	loadQuests();
}

CharacterCore* CharacterCore::createFromThis() {
	m_data.timePlayed += m_stopwatch.restart();
	return new CharacterCore(m_data);
}

CharacterCore::~CharacterCore() {
	delete m_weapon;
}

bool CharacterCore::load(const std::string& fileName) {
	CharacterCoreReader reader;

	if (!reader.readCharacterCore(fileName, m_data)) {
		return false;
	}

	loadWeapon();
	reloadAttributes();
	loadQuests();
	// measuring the time played with this save.
	m_stopwatch.restart();
	g_resourceManager->deleteItemResources();
	return true;
}

bool CharacterCore::quickload() {
	return load(g_documentsPath + GlobalResource::QUICKSAVE_PATH);
}

void CharacterCore::loadQuests() {
	m_quests.clear();
	for (auto& it : m_data.questStates) {
		QuestData data = QuestLoader::loadQuest(it.first);
		if (data.id.empty()) {
			g_logger->logError("CharacterCore::loadQuests", "Could not load quest: " + it.first);
			continue;
		}
		m_quests.insert({ data.id, data });
	}
}

void CharacterCore::loadNew() {
	// start map & position when a new game is loaded
	m_isNew = true;
	m_data.isInLevel = false;

	SpawnBean* spawn = g_databaseManager->getSpawnBean("start");
	m_data.currentMap = spawn->map_id;
	m_data.currentMapPosition = spawn->map_pos;
	m_data.attributes.currentHealthPoints = 100;
	m_data.attributes.maxHealthPoints = 100;
	m_data.hashValid = true;
	addItem(spawn->armor_id, 1);
	addItem(spawn->weapon_id, 1);
	equipItem(spawn->armor_id, ItemType::Equipment_body);
	equipItem(spawn->weapon_id, ItemType::Equipment_weapon);
	m_stopwatch.restart();
	g_resourceManager->deleteItemResources();
	reloadAttributes();
	delete spawn;
}

const std::string& CharacterCore::getEquippedItem(ItemType type) const {
	return m_data.equippedItems.at(type);
}

const Weapon* CharacterCore::getWeapon() const {
	return m_weapon;
}

QuestState CharacterCore::getQuestState(const std::string& id) const {
	if (contains(m_data.questStates, id)) {
		return m_data.questStates.at(id);
	}
	return QuestState::VOID;
}

bool CharacterCore::setQuestState(const std::string& id, QuestState state) {
	if (state == QuestState::Started && !contains(m_data.questStates, id)) {
		QuestData newQuest = QuestLoader::loadQuest(id);
		if (newQuest.id.empty()) {
			g_logger->logError("CharacterCore", "Could not load quest: " + id);
			return false;
		}
		m_quests.insert({ newQuest.id, newQuest });
		m_data.questStates.insert({ id, state });
		return true;
	}
	if (state != QuestState::Started && contains(m_data.questStates, id)) {
		if (m_data.questStates[id] == state) {
			return false;
		}
		m_data.questStates[id] = state;
		return true;
	}
	g_logger->logWarning("CharacterCore", "Cannot change quest state for quest: " + id + ". Either the quest has already started (and cannot be started again) or the quest has not yet started and needs to be started first.");
	return false;
}

void CharacterCore::setQuickslot(const std::string& item, int nr) {
	if (nr == 1) {
		m_data.quickSlot1 = item;
	}
	else if (nr == 2) {
		m_data.quickSlot2 = item;
	}
}

bool CharacterCore::save(const std::string& fileName, const std::string& name) {
	m_data.timePlayed += m_stopwatch.restart();
	m_data.dateSaved = time(nullptr);
	m_data.saveGameName = name;

	// write to savefile.
	CharacterCoreWriter writer;

	writer.createFile(fileName);
	return writer.saveToFile(fileName, m_data);
}

bool CharacterCore::quicksave() {
	return save(getDocumentsPath(GlobalResource::QUICKSAVE_PATH), "Quicksave");
}

bool CharacterCore::autosave() {
	return save(getDocumentsPath(GlobalResource::AUTOSAVE_PATH), "Autosave");
}

bool CharacterCore::createFile(const std::string& fileName) const {
	CharacterCoreWriter writer;
	return writer.createFile(fileName);
}

void CharacterCore::reloadAttributes() {
	m_totalAttributes = m_data.attributes;
	for (auto& it : m_data.equippedItems) {
		if (it.second.empty()) continue;
		Item* item = g_resourceManager->getItem(it.second);
		if (item == nullptr) continue;
		m_totalAttributes.addBean(item->getAttributes());
	}
	m_totalAttributes.currentHealthPoints = m_totalAttributes.maxHealthPoints;
}

void CharacterCore::reloadWeaponSlots() {
	if (m_weapon == nullptr) return;
	m_weapon->reload();
	for (int slot = 0; slot < static_cast<int>(m_data.equippedWeaponSlots.size()); ++slot) {
		m_weapon->addSpell(slot, m_data.equippedWeaponSlots[slot].first, false);
		for (int i = 0; i < static_cast<int>(m_data.equippedWeaponSlots[slot].second.size()); ++i) {
			m_weapon->addModifier(slot, i, m_data.equippedWeaponSlots[slot].second[i], false);
		}
	}

	std::string itemID = m_weapon->getID();
	m_data.weaponConfigurations[itemID] = m_data.equippedWeaponSlots;
}

void CharacterCore::loadWeapon() {
	delete m_weapon;
	m_weapon = nullptr;

	if (!m_data.equippedItems.at(ItemType::Equipment_weapon).empty()) {
		m_weapon = new Weapon(m_data.equippedItems.at(ItemType::Equipment_weapon));
		if (m_weapon->getType() != ItemType::Equipment_weapon) {
			delete m_weapon;
			m_weapon = nullptr;
			return;
		}
		// add equipped spells and their modifiers
		for (int slot = 0; slot < static_cast<int>(m_data.equippedWeaponSlots.size()); ++slot) {
			m_weapon->addSpell(slot, m_data.equippedWeaponSlots[slot].first, false);
			for (int i = 0; i < static_cast<int>(m_data.equippedWeaponSlots[slot].second.size()); ++i) {
				m_weapon->addModifier(slot, i, m_data.equippedWeaponSlots[slot].second[i], false);
			}
		}
	}
}

ExploredTiles& CharacterCore::getExploredTiles() {
	return m_data.tilesExplored;
}

bool CharacterCore::isMapExplored(const std::string& mapId) const {
	return contains(m_data.tilesExplored, mapId);
}

void CharacterCore::setEnemyKilled(const std::string& level, int pos) {
	m_data.enemiesKilled[level].insert(pos);
}

void CharacterCore::setEnemyLooted(const std::string& level, int pos) {
	m_data.enemiesLooted[level].insert(pos);
}

void CharacterCore::setItemLooted(const std::string& level, int pos) {
	m_data.itemsLooted[level].insert(pos);
}

void CharacterCore::setChestLooted(const std::string& level, int pos) {
	m_data.chestsLooted[level].insert(pos);
}

void CharacterCore::setTriggerTriggered(const std::string& world, int objectID) {
	m_data.triggersTriggered[world].insert(objectID);
}

void CharacterCore::setDoorOpen(const std::string& world, int objectID) {
	m_data.doorsOpen[world].insert(objectID);
}

void CharacterCore::setQuestTracked(const std::string& questID, bool isTracked) {
	if (isTracked) {
		m_data.questsTracked.insert(questID);
	} else {
		m_data.questsTracked.erase(questID);
	}
}

void CharacterCore::setWaypointUnlocked(const std::string& map, int objectID, const sf::Vector2f& pos) {
	m_data.waypointsUnlocked[map].insert({ objectID, pos });
	g_achievementManager->notifyAchievementCore(ACH_ALL_WAYPOINTS);
}

const QuestData* CharacterCore::getQuestData(const std::string& questID) const {
	if (!contains(m_quests, questID)) {
		g_logger->logError("CharacterCore", "Quest: " + questID + " has no quest data!");
		return nullptr;
	}
	return &m_quests.at(questID);
}

int CharacterCore::getNumberOfTargetsKilled(const std::string& questID, const std::string& name) const {
	if (!contains(m_data.questTargetProgress, questID) ||
		!contains(m_data.questTargetProgress.at(questID), name)) {
		return 0;
	}
	return m_data.questTargetProgress.at(questID).at(name);
}

int CharacterCore::getNumberOfTotalTargets(const std::string& questID, const std::string& name) const {
	if (!contains(m_quests, questID) ||
		!contains(m_quests.at(questID).targets, name)) {
		return 0;
	}
	return m_quests.at(questID).targets.at(name);
}

bool CharacterCore::isQuestComplete(const std::string& questID) const {
	auto state = getQuestState(questID);
	if (state != QuestState::Started && state != QuestState::Completed) return false;
	if (!contains(m_quests, questID)) {
		g_logger->logError("CharacterCore", "Quest: " + questID + " has no quest data!");
		return false;
	}
	QuestData data = m_quests.at(questID);

	// check quest conditions
	if (!data.conditions.empty()) {
		if (!contains(m_data.questConditionProgress, questID))
			return false;
		for (auto& it : data.conditions) {
			if (!contains(m_data.questConditionProgress.at(questID), it))
				return false;
		}
	}

	// check quest targets
	if (!data.targets.empty()) {
		if (!contains(m_data.questTargetProgress, questID))
			return false;
		for (auto& it : data.targets) {
			if (!contains(m_data.questTargetProgress.at(questID), it.first))
				return false;
			if (m_data.questTargetProgress.at(questID).at(it.first) < it.second)
				return false;
		}
	}

	// check collectibles
	if (!data.collectibles.empty()) {
		for (auto& it : data.collectibles) {
			if (!hasItem(it.first, it.second))
				return false;
		}
	}

	return true;
}

void CharacterCore::setQuestTargetKilled(const std::string& questID, const std::string& name) {
	if (!contains(m_data.questTargetProgress, questID)) {
		m_data.questTargetProgress.insert({ questID, std::map<std::string, int>() });
	}
	if (!contains(m_data.questTargetProgress.at(questID), name)) {
		m_data.questTargetProgress.at(questID).insert({ name, 0 });
	}
	m_data.questTargetProgress.at(questID).at(name) = m_data.questTargetProgress.at(questID).at(name) + 1;
}

void CharacterCore::setQuestConditionFulfilled(const std::string& questID, const std::string& condition) {
	if (!contains(m_data.questConditionProgress, questID)) {
		m_data.questConditionProgress.insert({ questID, std::set<std::string>() });
	}
	m_data.questConditionProgress.at(questID).insert(condition);
}

bool CharacterCore::isQuestConditionFulfilled(const std::string& questID, const std::string& condition) const {
	if (!contains(m_data.questConditionProgress, questID)) {
		return false;
	}
	return contains(m_data.questConditionProgress.at(questID), condition);
}

bool CharacterCore::isQuestDescriptionUnlocked(const std::string& questID, int description) const {
	if (!contains(m_data.questDescriptionProgress, questID)) {
		return false;
	}
	return contains(m_data.questDescriptionProgress.at(questID), description);
}

bool CharacterCore::isQuestTracked(const std::string& questID) const {
	return contains(m_data.questsTracked, questID);
}

bool CharacterCore::isSpellLearned(SpellID id) const {
	for (auto& type : m_data.spellsLearned) {
		for (auto spell : type.second) {
			if (spell == id) {
				return true;
			}
		}
	}
	return false;
}

bool CharacterCore::isSpellEquipped(SpellID id) const {
	for (auto& slot : m_data.equippedWeaponSlots) {
		if (slot.first == id) {
			return true;
		}
	}
	return false;
}

bool CharacterCore::isWaypointUnlocked(const std::string& mapId, int objectId) const {
	return contains(m_data.waypointsUnlocked, mapId) && contains(m_data.waypointsUnlocked.at(mapId), objectId);
}

bool CharacterCore::isItemLooted(const std::string& levelId, int position) const {
	return contains(m_data.itemsLooted, levelId) && contains(m_data.itemsLooted.at(levelId), position);
}

bool CharacterCore::setConditionFulfilled(const std::string& conditionType, const std::string& condition) {
	if (!contains(m_data.conditionProgress, conditionType)) {
		m_data.conditionProgress.insert({ conditionType, std::set<std::string>() });
	}
	const auto& ret = m_data.conditionProgress.at(conditionType).insert(condition);
	return ret.second;
}

bool CharacterCore::unlockQuestDescription(const std::string& questID, int descriptionID) {
	if (!contains(m_data.questDescriptionProgress, questID)) {
		m_data.questDescriptionProgress.insert({ questID, std::set<int>() });
	}
	const auto& ret = m_data.questDescriptionProgress.at(questID).insert(descriptionID);
	return ret.second;
}

bool CharacterCore::isConditionFulfilled(const std::string& conditionType, const std::string& condition) const {
	if (!contains(m_data.conditionProgress, conditionType)) {
		return false;
	}

	return contains(m_data.conditionProgress.at(conditionType), condition);
}

bool CharacterCore::isConditionsFulfilled(const std::vector<Condition>& conditions) const {
	for (auto& cond : conditions) {
		if (cond.negative) {
			if (isConditionFulfilled(cond.type, cond.name)) return false;
		}
		else {
			if (!isConditionFulfilled(cond.type, cond.name)) return false;
		}
	}

	return true;
}

bool CharacterCore::isEnemyKilled(const std::string& levelID, int objectID) const {
	if (!contains(m_data.enemiesKilled, levelID)) return false;
	if (!contains(m_data.enemiesKilled.at(levelID), objectID)) return false;
	return true;
}

bool CharacterCore::isEnemyLooted(const std::string& levelID, int objectID) const {
	if (!contains(m_data.enemiesLooted, levelID)) return false;
	if (!contains(m_data.enemiesLooted.at(levelID), objectID)) return false;
	return true;
}

bool CharacterCore::isDoorOpen(const std::string& world, int objectID) {
	if (!contains(m_data.doorsOpen, world)) return false;
	if (!contains(m_data.doorsOpen.at(world), objectID)) return false;
	return true;
}

bool CharacterCore::isTriggerTriggered(const std::string& worldID, int objectID) {
	if (!contains(m_data.triggersTriggered, worldID)) return false;
	if (!contains(m_data.triggersTriggered.at(worldID), objectID)) return false;
	return true;
}

int CharacterCore::getItemAmount(const std::string& itemID) const {
	if (itemID.empty()) return 0;

	if (itemID == "gold") {
		return m_data.gold;
	}

	int foundAmount = 0;
	for (auto& item : m_data.equippedItems) {
		if (item.second == itemID) {
			foundAmount++;
		}
	}

	if (contains(m_data.items, itemID))
		foundAmount += m_data.items.at(itemID);

	return foundAmount;
}

int CharacterCore::getStoredItemAmount(const std::string& itemID) const {
	if (itemID.empty()) return 0;

	if (itemID == "gold") {
		return m_data.storedGold;
	}

	int foundAmount = 0;
	if (contains(m_data.storedItems, itemID))
		foundAmount += m_data.storedItems.at(itemID);

	return foundAmount;
}

bool CharacterCore::hasStoredItem(const std::string& itemID, int amount) const {
	if (amount <= 0) return false;

	return getStoredItemAmount(itemID) >= amount;
}

bool CharacterCore::hasItem(const std::string& itemID, int amount) const {
	if (amount <= 0) return false;

	return getItemAmount(itemID) >= amount;
}

bool CharacterCore::isItemEquipped(const std::string& itemID) const {
	for (auto& it : m_data.equippedItems) {
		if (it.second == itemID) {
			return true;
		}
	}

	return false;
}

const CharacterCoreData& CharacterCore::getData() const {
	return m_data;
}

MerchantData CharacterCore::getMerchantData(const std::string& merchantID) {
	MerchantData data = MerchantLoader::loadMerchant(merchantID, this);
	if (contains(m_data.merchantStates, merchantID)) {
		data.wares = m_data.merchantStates.at(merchantID);
	}

	return data;
}

void CharacterCore::setMerchantData(const std::string& merchantID, const std::map<std::string, int>& wares) {
	m_data.merchantStates[merchantID] = wares;
}

void CharacterCore::addPermanentAttributes(const AttributeData& attributes) {
	m_data.attributes.addBean(attributes);
	reloadAttributes();
}

void CharacterCore::learnModifier(SpellModifierType modifierType, const std::string& levelID, int objectID) {
	if (objectID > -1) {
		m_data.modifiersUnlocked[levelID].insert(objectID);
	}

	if (!contains(m_data.modfiersLearned, modifierType)) {
		m_data.modfiersLearned.insert({ modifierType, 0 });
	}
	m_data.modfiersLearned[modifierType] = std::min(3, m_data.modfiersLearned[modifierType] + 1);
	g_achievementManager->notifyAchievementCore(ACH_ALL_MODIFIERS);
}

void CharacterCore::learnHint(const std::string& hintKey) {
	if (!contains(m_data.hintsLearned, hintKey)) {
		m_data.hintsLearned.push_back(hintKey);
	}
}

void CharacterCore::setWeather(const std::string& worldID, const WeatherData& data) {
	m_data.currentWeather[worldID] = data;
}

const WeatherData* CharacterCore::getWeather(const std::string& worldID) const {
	if (!contains(m_data.currentWeather, worldID)) {
		return nullptr;
	}
	return &m_data.currentWeather.at(worldID);
}

void CharacterCore::addReputation(FractionID fraction, int amount) {
	if (fraction == FractionID::VOID || amount < 0) return;

	if (!contains(m_data.reputationProgress, fraction)) {
		m_data.reputationProgress.insert({ fraction, 0 });
	}

	m_data.reputationProgress.at(fraction) = std::min(100, m_data.reputationProgress.at(fraction) + amount);
	g_achievementManager->notifyAchievementCore(ACH_CLERIC_REP_70);
	g_achievementManager->notifyAchievementCore(ACH_NECRO_REP_70);
	g_achievementManager->notifyAchievementCore(ACH_THIEF_REP_70);
}

void CharacterCore::setGuild(FractionID fraction) {
	m_data.guild = fraction;
}

void CharacterCore::setWeaponSpell(Key key) {
	m_data.weaponSpell = key;
}

int CharacterCore::getReputation(FractionID fraction) const {
	if (!contains(m_data.reputationProgress, fraction))
		return 0;

	return m_data.reputationProgress.at(fraction);
}

AttributeData CharacterCore::getTotalAttributes() const {
	return m_totalAttributes;
}

std::map<std::string, int>* CharacterCore::getItems() {
	return &(m_data.items);
}

void CharacterCore::addGold(int gold) {
	m_data.gold += std::max(gold, 0);
	g_achievementManager->notifyAchievementCore(ACH_GOLD_1000);
}

void CharacterCore::removeGold(int gold) {
	m_data.gold -= std::min(m_data.gold, gold);
}

void CharacterCore::notifyItemChange(const std::string& itemID, int amount) {
	if (itemID.empty()) return;

	if (itemID == "gold") {
		if (amount < 0) {
			removeGold(-amount);
		}
		else {
			addGold(amount);
		}
	}
	else {
		if (amount < 0) {
			removeItem(itemID, -amount);
		}
		else {
			addItem(itemID, amount);
		}
	}
}

void CharacterCore::addItem(const std::string& item, int quantity) {
	if (item.empty()) return;

	if (contains(m_data.items, item)) {
		m_data.items.at(item) = m_data.items.at(item) + quantity;
	}
	else {
		m_data.items.insert({ item, quantity });
	}

	g_achievementManager->notifyAchievementCore(ACH_ALL_KEYS);
	g_achievementManager->notifyAchievementCore(ACH_MASOCHIST);
}

void CharacterCore::removeItem(const std::string& item, int quantity) {
	if (item.empty()) return;
	int quantityEreased = 0;

	if (contains(m_data.items, item)) {
		m_data.items.at(item) = m_data.items.at(item) - quantity;
		if (m_data.items.at(item) <= 0) {
			quantityEreased = quantity + m_data.items.at(item);
			m_data.items.erase(item);
		}
		else {
			quantityEreased = quantity;
		}
	}

	// also look for equipped items
	if (quantityEreased < quantity) {
		for (auto& eqItem : m_data.equippedItems) {
			if (eqItem.second == item) {
				eqItem.second.clear();
				++quantityEreased;
			}
			if (quantityEreased == quantity) break;
		}
	}
}

void CharacterCore::addStoredItem(const std::string& item, int quantity) {
	if (item.empty()) return;

	if (contains(m_data.storedItems, item)) {
		m_data.storedItems.at(item) = m_data.storedItems.at(item) + quantity;
	}
	else {
		m_data.storedItems.insert({ item, quantity });
	}
}

void CharacterCore::removeStoredItem(const std::string& item, int quantity) {
	if (item.empty()) return;

	if (contains(m_data.storedItems, item)) {
		m_data.storedItems.at(item) = m_data.storedItems.at(item) - quantity;
		if (m_data.storedItems.at(item) <= 0) {
			m_data.storedItems.erase(item);
		}
	}
}

void CharacterCore::setOverworld(const std::string& map) {
	m_data.lastOverworldMap = map;
}

void CharacterCore::setMap(const sf::Vector2f& position, const std::string& map) {
	m_data.currentMap = map;
	m_data.currentMapPosition = position;
	m_data.isInLevel = false;
}

void CharacterCore::setLevel(const sf::Vector2f& position, const std::string& level) {
	m_data.currentLevel = level;
	m_data.currentLevelPosition = position;
	m_data.isInLevel = true;
}

void CharacterCore::setForcedMap(const sf::Vector2f& position, const std::string& map) {
	m_data.forcedMap = map;
	m_data.forcedMapPosition = position;
}

void CharacterCore::replaceForcedMap() {
	if (m_data.forcedMap.empty()) return;

	m_data.currentMap = m_data.forcedMap;
	m_data.currentMapPosition = m_data.forcedMapPosition;

	m_data.forcedMap.clear();
}

void CharacterCore::setInLevel(bool inLevel) {
	m_data.isInLevel = inLevel;
}

void CharacterCore::removeModifier(int slotNr, int modifierNr) {
	if (slotNr < 0 || slotNr + 1 > static_cast<int>(m_data.equippedWeaponSlots.size())) return;
	std::vector<SpellModifier>& modifiers = m_data.equippedWeaponSlots.at(slotNr).second;
	if (modifierNr < 0 || modifierNr + 1 > static_cast<int>(modifiers.size())) return;

	modifiers[modifierNr].type = SpellModifierType::VOID;

	reloadWeaponSlots();
}

void CharacterCore::removeSpell(int slotNr) {
	if (slotNr < 0 || slotNr + 1 > static_cast<int>(m_data.equippedWeaponSlots.size())) return;
	m_data.equippedWeaponSlots.at(slotNr).first = SpellID::VOID;
	m_data.equippedWeaponSlots.at(slotNr).second.clear();
	reloadWeaponSlots();
}

void CharacterCore::addSpell(SpellID id, int slotNr) {
	if (m_weapon == nullptr) return;
	if (!m_weapon->isSpellAllowed(slotNr, id)) return;
	// check if this spell is already in another slot, if yes, remove that
	for (auto& it : m_data.equippedWeaponSlots) {
		if (it.first == id) {
			it.first = SpellID::VOID;
			it.second.clear();
		}
	}

	while (static_cast<int>(m_data.equippedWeaponSlots.size()) < slotNr + 1) {
		m_data.equippedWeaponSlots.push_back(std::pair<SpellID, std::vector<SpellModifier>>({ SpellID::VOID, std::vector<SpellModifier>() }));
	}

	m_data.equippedWeaponSlots.at(slotNr).first = id;
	m_data.equippedWeaponSlots.at(slotNr).second.clear();

	reloadWeaponSlots();
}

void CharacterCore::addModifier(const SpellModifier& modifier, int slotNr, int modifierNr) {
	if (m_weapon == nullptr) return;
	if (m_weapon->addModifier(slotNr, modifierNr, modifier, true)) {
		// check if this spell slot exists. If not, fill the slots
		if (static_cast<int>(m_data.equippedWeaponSlots.size()) < slotNr + 1) {
			for (size_t i = m_data.equippedWeaponSlots.size(); static_cast<int>(i) < slotNr + 1; ++i) {
				std::pair<SpellID, std::vector<SpellModifier>> newSlot;
				newSlot.first = SpellID::VOID;
				m_data.equippedWeaponSlots.push_back(newSlot);
			}
		}
		std::pair<SpellID, std::vector<SpellModifier>>& slot = m_data.equippedWeaponSlots.at(slotNr);
		// check if this modifier slot exist
		if (static_cast<int>(slot.second.size()) < modifierNr + 1) {
			g_logger->logInfo("CharacterCore", "Adding empty modifier slots slots to the weapon!");
			for (size_t i = slot.second.size(); static_cast<int>(i) < modifierNr + 1; ++i) {
				slot.second.push_back(SpellModifier());
			}
		}
		for (auto& it : slot.second) {
			if (it.type == modifier.type) {
				it.type = SpellModifierType::VOID;;
			}
		}

		// add the modifier
		slot.second[modifierNr] = modifier;
	}
	reloadWeaponSlots();
}

void CharacterCore::learnSpell(SpellID id) {
	SpellType type = SpellData::getSpellData(id).spellType;
	if (!contains(m_data.spellsLearned, type)) {
		m_data.spellsLearned.insert({ type, std::set<SpellID>() });
	}
	m_data.spellsLearned.at(type).insert(id);
}

void CharacterCore::equipItem(const std::string& item, ItemType type) {
	if (!contains(m_data.equippedItems, type))
		return;

	std::string oldItem = m_data.equippedItems.at(type);
	m_data.equippedItems.at(type) = item;

	// special case for weapon
	if (type == ItemType::Equipment_weapon) {
		m_data.equippedWeaponSlots.clear();
		loadWeapon();
		if (m_weapon && contains(m_data.weaponConfigurations, m_weapon->getID())) {
			m_data.equippedWeaponSlots = m_data.weaponConfigurations[m_weapon->getID()];
		}
		else if (item.empty()) {
			auto realItem = g_resourceManager->getItem(item);
			if (realItem) {
				for (size_t i = 0; i < realItem->getBeans<ItemWeaponSlotBean>().size(); ++i) {
					m_data.equippedWeaponSlots.push_back(std::pair<SpellID, std::vector<SpellModifier>>(SpellID::VOID, std::vector<SpellModifier>()));
				}
			}
		}
		reloadWeaponSlots();
	}

	removeItem(item, 1);
	if (!oldItem.empty()) {
		addItem(oldItem, 1);
	}

	reloadAttributes();
}

bool CharacterCore::isAutosave() {
	return m_isAutosave;
}

void CharacterCore::setAutosave(bool value) {
	m_isAutosave = value;
}

void CharacterCore::setCharacterJailed() {
	SpawnBean* spawn = g_databaseManager->getSpawnBean("prison");
	m_data.isInLevel = false;
	m_data.currentMap = spawn->map_id;
	m_data.currentMapPosition = spawn->map_pos;

	// store the gold
	m_data.storedGold += m_data.gold;
	m_data.gold = 0;

	// store the items
	for (auto& it : m_data.items) {
		if (!contains(m_data.storedItems, it.first)) {
			m_data.storedItems.insert({ it.first, it.second });
		}
		else {
			m_data.storedItems.at(it.first) += it.second;
		}
	}
	m_data.items.clear();

	// store the equipment
	for (auto& it : m_data.equippedItems) {
		addStoredItem(it.second, 1);
		it.second.clear();
	}

	// if the items already exists, they will not be given anew.
	removeStoredItem(spawn->weapon_id, 1);
	removeStoredItem(spawn->armor_id, 1);

	// equip the new armor.
	addItem(spawn->armor_id, 1);
	addItem(spawn->weapon_id, 1);
	equipItem(spawn->armor_id, ItemType::Equipment_body);
	equipItem(spawn->weapon_id, ItemType::Equipment_weapon);

	// achievement for getting arrested
	g_achievementManager->unlockAchievement(ACH_CRIMINAL);

	delete spawn;
}

const std::map<std::string, int>& CharacterCore::getStoredItems() const {
	return m_data.storedItems;
}

int CharacterCore::getStoredGold() const {
	return m_data.storedGold;
}

void CharacterCore::resetStoredItems() {
	m_data.storedGold = 0;
	m_data.storedItems.clear();
}

void CharacterCore::setAchievementUnlocked(const std::string& achievement) {
	m_data.achievementsUnlocked.insert(achievement);
}

void CharacterCore::setBookRead(const std::string& itemId) {
	m_data.booksRead.insert(itemId);
	g_achievementManager->notifyAchievementCore(ACH_ALL_BOOKS);
}

void CharacterCore::increaseDeathCount(const std::string& level) {
	m_data.deaths++;

	if (!contains(m_data.levelDeaths, level)) return;
	m_data.levelDeaths.at(level)++;
}

void CharacterCore::setHashInvalid() {
	m_data.hashValid = false;
}
