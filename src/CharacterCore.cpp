#include "CharacterCore.h"
#include "Item.h"
#include "FileIO/MerchantLoader.h"
#include "GlobalResource.h"

using namespace std;

CharacterCore::CharacterCore() {
	m_data = DEFAULT_CORE;

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
	m_data.currentMap = "res/map/meadows/meadows.tmx";
	m_data.currentMapPosition = sf::Vector2f(1850.f, 700.f);
	m_data.attributes.currentHealthPoints = 100;
	m_data.attributes.maxHealthPoints = 100;
	m_data.attributes.critical = 5;
	m_data.equippedItems.at(ItemType::Equipment_body) = "eq_defaultarmor";
	setQuestState("who_am_i", QuestState::Started);
	m_stopwatch.restart();
	g_resourceManager->deleteItemResources();
	reloadAttributes();
}

const std::string& CharacterCore::getEquippedItem(ItemType type) {
	return m_data.equippedItems.at(type);
}

const Weapon* CharacterCore::getWeapon() {
	return m_weapon;
}

QuestState CharacterCore::getQuestState(const std::string& id) const {
	if (m_data.questStates.find(id) != m_data.questStates.end()) {
		return m_data.questStates.at(id);
	}
	return QuestState::VOID;
}

void CharacterCore::setQuestState(const std::string& id, QuestState state) {
	if (state == QuestState::Started && m_data.questStates.find(id) == m_data.questStates.end()) {
		QuestData newQuest = QuestLoader::loadQuest(id);
		if (newQuest.id.empty()) {
			g_logger->logError("CharacterCore", "Could not load quest: " + id);
			return;
		}
		m_quests.insert({ newQuest.id, newQuest });
		m_data.questStates.insert({ id, state });
		return;
	}
	if (state != QuestState::Started && m_data.questStates.find(id) != m_data.questStates.end()) {
		m_data.questStates[id] = state;
		return;
	}
	g_logger->logError("CharacterCore", "Cannot change quest state for quest: " + id + ". Either the quest has already started (and cannot be started again) or the quest has not yet started and needs to be started first.");
}

void CharacterCore::setQuickslot(const std::string& item, int nr) {
	if (nr == 1) {
		m_data.quickSlot1 = item;
	}
	else if (nr == 2) {
		m_data.quickSlot2 = item;
	}
}

bool CharacterCore::save(const std::string& fileName, const string& name) {
	m_data.timePlayed += m_stopwatch.restart();
	m_data.dateSaved = time(nullptr);
	m_data.saveGameName = name;

	// write to savefile.
	CharacterCoreWriter writer;

	writer.createFile(fileName);
	return writer.saveToFile(fileName, m_data);
}

bool CharacterCore::quicksave() {
	return save(g_documentsPath + GlobalResource::QUICKSAVE_PATH, "Quicksave");
}

bool CharacterCore::autosave() {
	return save(g_documentsPath + GlobalResource::AUTOSAVE_PATH, "Autosave");
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
		if (item == nullptr || !item->isEquipmentItem()) continue;
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

void CharacterCore::initializeLevelMaps(const std::string& level) {
	// if these entries for the given level already exist, an insert will do nothing.
	m_data.enemiesKilled.insert({ level, std::set<int>() });
	m_data.enemiesLooted.insert({ level, std::set<int>() });
	m_data.itemsLooted.insert({ level, std::set<int>() });
	m_data.chestsLooted.insert({ level, std::set<int>() });
	m_data.triggersTriggered.insert({ level, std::set<int>() });
}

void CharacterCore::initializeMapMaps(const std::string& map) {
	// if these entries for the given map already exist, an insert will do nothing.
	m_data.waypointsUnlocked.insert({ map, std::set<int>() });
	m_data.triggersTriggered.insert({ map, std::set<int>() });
}

std::map<std::string, std::vector<bool>>& CharacterCore::getExploredTiles() {
	return m_data.tilesExplored;
}

void CharacterCore::setEnemyKilled(const std::string& level, int pos) {
	m_data.enemiesKilled.at(level).insert(pos);
}

void CharacterCore::setEnemyLooted(const std::string& level, int pos) {
	m_data.enemiesLooted.at(level).insert(pos);
}

void CharacterCore::setItemLooted(const std::string& level, int pos) {
	m_data.itemsLooted.at(level).insert(pos);
}

void CharacterCore::setChestLooted(const std::string& level, int pos) {
	m_data.chestsLooted.at(level).insert(pos);
}

void CharacterCore::setTriggerTriggered(const std::string& world, int pos) {
	m_data.triggersTriggered.at(world).insert(pos);
}

void CharacterCore::setWaypointUnlocked(const std::string& map, int pos) {
	m_data.waypointsUnlocked.at(map).insert(pos);
}

const QuestData* CharacterCore::getQuestData(const std::string& questID) const {
	if (m_quests.find(questID) == m_quests.end()) {
		g_logger->logError("CharacterCore", "Quest: " + questID + " has no quest data!");
		return nullptr;
	}
	return &m_quests.at(questID);
}

int CharacterCore::getNumberOfTargetsKilled(const std::string& questID, const std::string& name) const {
	if (m_data.questTargetProgress.find(questID) == m_data.questTargetProgress.end() ||
		m_data.questTargetProgress.at(questID).find(name) == m_data.questTargetProgress.at(questID).end()) {
		return 0;
	}
	return m_data.questTargetProgress.at(questID).at(name);
}

int CharacterCore::getNumberOfTotalTargets(const std::string& questID, const std::string& name) const {
	if (m_quests.find(questID) == m_quests.end() ||
		m_quests.at(questID).targets.find(name) == m_quests.at(questID).targets.end()) {
		return 0;
	}
	return m_quests.at(questID).targets.at(name);
}

bool CharacterCore::isQuestComplete(const std::string& questID) const {
	if (getQuestState(questID) != QuestState::Started) return false;
	if (m_quests.find(questID) == m_quests.end()) {
		g_logger->logError("CharacterCore", "Quest: " + questID + " has no quest data!");
		return false;
	}
	QuestData data = m_quests.at(questID);

	// check quest conditions
	if (!data.conditions.empty()) {
		if (m_data.questConditionProgress.find(questID) == m_data.questConditionProgress.end())
			return false;
		for (auto& it : data.conditions) {
			if (m_data.questConditionProgress.at(questID).find(it) == m_data.questConditionProgress.at(questID).end())
				return false;
		}
	}

	// check quest targets
	if (!data.targets.empty()) {
		if (m_data.questTargetProgress.find(questID) == m_data.questTargetProgress.end())
			return false;
		for (auto& it : data.targets) {
			if (m_data.questTargetProgress.at(questID).find(it.first) == m_data.questTargetProgress.at(questID).end())
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
	if (m_data.questTargetProgress.find(questID) == m_data.questTargetProgress.end()) {
		m_data.questTargetProgress.insert({ questID, std::map<std::string, int>() });
	}
	if (m_data.questTargetProgress.at(questID).find(name) == m_data.questTargetProgress.at(questID).end()) {
		m_data.questTargetProgress.at(questID).insert({ name, 0 });
	}
	m_data.questTargetProgress.at(questID).at(name) = m_data.questTargetProgress.at(questID).at(name) + 1;
}

void CharacterCore::setQuestConditionFulfilled(const std::string& questID, const std::string& condition) {
	if (m_data.questConditionProgress.find(questID) == m_data.questConditionProgress.end()) {
		m_data.questConditionProgress.insert({ questID, std::set<std::string>() });
	}
	m_data.questConditionProgress.at(questID).insert(condition);
}

bool CharacterCore::isQuestConditionFulfilled(const std::string& questID, const std::string& condition) const {
	if (m_data.questConditionProgress.find(questID) == m_data.questConditionProgress.end()) {
		return false;
	}
	return m_data.questConditionProgress.at(questID).find(condition) != m_data.questConditionProgress.at(questID).end();
}

void CharacterCore::setConditionFulfilled(const std::string& conditionType, const std::string& condition) {
	if (m_data.conditionProgress.find(conditionType) == m_data.conditionProgress.end()) {
		m_data.conditionProgress.insert({ conditionType, std::set<std::string>() });
	}
	m_data.conditionProgress.at(conditionType).insert(condition);
}

void CharacterCore::unlockQuestDescription(const std::string& questID, int descriptionID) {
	if (m_data.questDescriptionProgress.find(questID) == m_data.questDescriptionProgress.end()) {
		m_data.questDescriptionProgress.insert({ questID, std::set<int>() });
	}
	m_data.questDescriptionProgress.at(questID).insert(descriptionID);
}

bool CharacterCore::isConditionFulfilled(const std::string& conditionType, const std::string& condition) const {
	if (m_data.conditionProgress.find(conditionType) == m_data.conditionProgress.end()) {
		return false;
	}
	const std::set<std::string>& conditions = m_data.conditionProgress.at(conditionType);
	return (conditions.find(condition) != conditions.end());
}

bool CharacterCore::isEnemyKilled(const std::string& levelID, int objectID) const {
	if (m_data.enemiesKilled.find(levelID) == m_data.enemiesKilled.end()) return false;
	if (m_data.enemiesKilled.at(levelID).find(objectID) == m_data.enemiesKilled.at(levelID).end()) return false;
	return true;
}

bool CharacterCore::isEnemyLooted(const std::string& levelID, int objectID) const {
	if (m_data.enemiesLooted.find(levelID) == m_data.enemiesLooted.end()) return false;
	if (m_data.enemiesLooted.at(levelID).find(objectID) == m_data.enemiesLooted.at(levelID).end()) return false;
	return true;
}

bool CharacterCore::isTriggerTriggered(const std::string& worldID, int objectID) {
	if (m_data.triggersTriggered.find(worldID) == m_data.triggersTriggered.end()) return false;
	if (m_data.triggersTriggered.at(worldID).find(objectID) == m_data.triggersTriggered.at(worldID).end()) return false;
	return true;
}

bool CharacterCore::hasItem(const std::string& itemID, int amount) const {
	if (itemID.empty()) return false;

	if (itemID.compare("gold") == 0) {
		// check for gold
		return m_data.gold >= amount;
	}

	int foundAmount = 0;
	for (auto& item : m_data.equippedItems) {
		if (item.second.compare(itemID) == 0) {
			foundAmount++;
		}
	}
	
	if (m_data.items.find(itemID) != m_data.items.end())
		foundAmount += m_data.items.at(itemID);

	return foundAmount >= amount;
}

const CharacterCoreData& CharacterCore::getData() const {
	return m_data;
}

MerchantData CharacterCore::getMerchantData(const std::string& merchantID) {
	if (m_data.merchantStates.find(merchantID) != m_data.merchantStates.end()) {
		return m_data.merchantStates.at(merchantID);
	}

	return MerchantLoader::loadMerchant(merchantID);
}

void CharacterCore::setMerchantData(const std::string& merchantID, const MerchantData& data) {
	m_data.merchantStates[merchantID] = data;
}

void CharacterCore::addPermanentAttributes(const AttributeData& attributes) {
	m_data.attributes.addBean(attributes);
	reloadAttributes();
}

void CharacterCore::learnModifier(const SpellModifier& modifier) {
	if (m_data.modfiersLearned.find(modifier.type) == m_data.modfiersLearned.end()) {
		m_data.modfiersLearned.insert({ modifier.type, modifier.level});
	}
	else {
		m_data.modfiersLearned[modifier.type] = std::max(m_data.modfiersLearned[modifier.type], modifier.level);
	}	
}

void CharacterCore::learnHint(const std::string& hintKey) {
	if (std::find(m_data.hintsLearned.begin(), m_data.hintsLearned.end(), hintKey) == m_data.hintsLearned.end()) {
		m_data.hintsLearned.push_back(hintKey);
	}
}

void CharacterCore::setWeather(const std::string& worldID, const WeatherData& data) {
	m_data.currentWeather[worldID] = data;
}

const WeatherData* CharacterCore::getWeather(const std::string& worldID) const {
	if (m_data.currentWeather.find(worldID) == m_data.currentWeather.end()) {
		return nullptr;
	}
	return &m_data.currentWeather.at(worldID);
}

void CharacterCore::addReputation(FractionID fraction, int amount) {
	if (fraction == FractionID::VOID || amount < 0) return;

	if (m_data.reputationProgress.find(fraction) == m_data.reputationProgress.end()) {
		m_data.reputationProgress.insert({ fraction, 0 });
	}

	m_data.reputationProgress.at(fraction) = std::min(100, m_data.reputationProgress.at(fraction) + amount);
}

int CharacterCore::getReputation(FractionID fraction) const {
	if (m_data.reputationProgress.find(fraction) == m_data.reputationProgress.end())
		return 0;

	return m_data.reputationProgress.at(fraction);
}

const AttributeData& CharacterCore::getTotalAttributes() const {
	return m_totalAttributes;
}

std::map<std::string, int>* CharacterCore::getItems() {
	return &(m_data.items);
}

void CharacterCore::addGold(int gold) {
	m_data.gold += std::max(gold, 0);
}

void CharacterCore::removeGold(int gold) {
	m_data.gold -= std::min(m_data.gold, gold);
}

void CharacterCore::notifyItemChange(const std::string& itemID, int amount) {
	if (itemID.empty()) return;
	
	if (itemID.compare("gold") == 0) {
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
	auto it = m_data.items.find(item);
	
	if (it != m_data.items.end()) {
		m_data.items.at(item) = m_data.items.at(item) + quantity;
	}
	else {
		m_data.items.insert({ item, quantity });
	}
}

void CharacterCore::removeItem(const std::string& item, int quantity) {
	int quantityEreased = 0;

	auto it = m_data.items.find(item);

	if (it != m_data.items.end()) {
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
			if (eqItem.second.compare(item) == 0) {
				eqItem.second.clear();
				++quantityEreased;
			}
			if (quantityEreased == quantity) break;
		}
	}
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
		m_data.equippedWeaponSlots.push_back(std::pair<SpellID, std::vector<SpellModifier>>({SpellID::VOID, std::vector<SpellModifier>()}));
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
				slot.second.push_back(EMPTY_SPELLMODIFIER);
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
	if (m_data.spellsLearned.find(type) == m_data.spellsLearned.end()) {
		m_data.spellsLearned.insert({ type, std::set<SpellID>() });
	}
	m_data.spellsLearned.at(type).insert(id);
}

void CharacterCore::equipItem(const std::string& item, ItemType type) {
	if (m_data.equippedItems.find(type) == m_data.equippedItems.end())
		return;
	std::string oldItem = m_data.equippedItems.at(type);
	m_data.equippedItems.at(type) = item;

	// special case for weapon
	if (type == ItemType::Equipment_weapon) {
		m_data.equippedWeaponSlots.clear();
		loadWeapon();
		if (m_weapon && m_data.weaponConfigurations.find(m_weapon->getID()) != m_data.weaponConfigurations.end()) {
			m_data.equippedWeaponSlots = m_data.weaponConfigurations[m_weapon->getID()];
		}
		else if (item.empty()) {
			std::vector<ItemWeaponSlotBean> wep = g_databaseManager->getItemWeaponSlotBeans(item);
			for (size_t i = 0; i < wep.size(); ++i) {
				m_data.equippedWeaponSlots.push_back(std::pair<SpellID, std::vector<SpellModifier>>(SpellID::VOID, std::vector<SpellModifier>()));
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

bool CharacterCore::isNewGame() {
	if (m_isNew) {
		m_isNew = false;
		return true;
	}
	return m_isNew;
}
