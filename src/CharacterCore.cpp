#include "CharacterCore.h"
#include "Item.h"
#include "FileIO/MerchantLoader.h"

using namespace std;

const char* CharacterCore::QUICKSAVE_LOCATION = "saves/quicksave.sav";
const char* CharacterCore::DEBUGSAVE_LOCATION = "saves/debug.sav";

CharacterCore::CharacterCore() {
	m_data = DEFAULT_CORE;
}

CharacterCore::CharacterCore(const CharacterCoreData& data) {
	m_data = data;
	m_stopwatch.restart();
	reloadAttributes();
	loadQuests();
}

CharacterCore::~CharacterCore() {
	clearEquippedItems();
	clearItems();
}

bool CharacterCore::quickload() {
	return load(QUICKSAVE_LOCATION);
}

bool CharacterCore::load(const std::string& fileName) {
	CharacterCoreReader reader;

	if (!reader.readCharacterCore(fileName, m_data)) {
		return false;
	}

	// measuring the time played with this save.
	m_stopwatch.restart();
	reloadAttributes();
	loadQuests();
	return true;
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
	m_data.isInLevel = false;
	m_data.currentMap = "res/map/meadows/meadows.tmx";
	m_data.currentMapPosition = sf::Vector2f(1850.f, 700.f);
	m_data.attributes.currentHealthPoints = 100;
	m_data.attributes.maxHealthPoints = 100;
	m_data.attributes.critical = 5;
	setQuestState("who_am_i", QuestState::Started);
	m_stopwatch.restart();
	reloadAttributes();
}

const Item* CharacterCore::getEquippedItem(ItemType type) {
	if (m_equippedItems.empty()) {
		loadEquipmentItems();
	}
	return m_equippedItems.at(type);
}

const Item* CharacterCore::getItem(const std::string& id) {
	if (m_items.empty()) {
		loadItems();
	}
	if (m_items.find(id) == m_items.end()) {
		g_logger->logError("CharacterCore", "Item with id: " + id + " does not exist!");
		return nullptr;
	}

	return &m_items.at(id);
}

const Weapon* CharacterCore::getWeapon() {
	const Weapon* weapon;
	if (!(weapon = dynamic_cast<const Weapon*>(getEquippedItem(ItemType::Equipment_weapon)))) {
		// unexpected
		return nullptr;
	}
	return weapon;
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
	m_data.timePlayed += m_stopwatch.restart();
	m_data.dateSaved = time(nullptr);
	m_data.saveGameName = "Quicksave";

	// write to savefile.
	CharacterCoreWriter writer;
	writer.createFile(QUICKSAVE_LOCATION);
	return writer.saveToFile(QUICKSAVE_LOCATION, m_data);
}

bool CharacterCore::createFile(const std::string& fileName) const {
	CharacterCoreWriter writer;
	return writer.createFile(fileName);
}

void CharacterCore::reloadAttributes() {
	m_totalAttributes = m_data.attributes;
	loadEquipmentItems();
	for (auto &it : m_equippedItems) {
		if (it.second == nullptr) continue;
		m_totalAttributes.addBean(it.second->getAttributes());
	}
	m_totalAttributes.currentHealthPoints = m_totalAttributes.maxHealthPoints;
}

void CharacterCore::reloadWeaponSlots() {
	Weapon* wep = dynamic_cast<Weapon*>(m_equippedItems.at(ItemType::Equipment_weapon));
	if (wep == nullptr) return;
	wep->reload();
	for (int slot = 0; slot < m_data.equippedWeaponSlots.size(); slot++) {
		wep->addSpell(slot, m_data.equippedWeaponSlots[slot].first, false);
		for (int i = 0; i < m_data.equippedWeaponSlots[slot].second.size(); ++i) {
			wep->addModifier(slot, i, m_data.equippedWeaponSlots[slot].second[i], false);
		}
	}
}

void CharacterCore::loadEquipmentItems() {
	clearEquippedItems();

	Weapon* eqWeapon = nullptr;
	Item* eqBack = nullptr;
	Item* eqBody = nullptr;
	Item* eqHead = nullptr;
	Item* eqNeck = nullptr;
	Item* eqRing1 = nullptr;
	Item* eqRing2 = nullptr;
	if (!m_data.equippedWeapon.empty() && g_databaseManager->itemExists(m_data.equippedWeapon)) {
		eqWeapon = new Weapon(m_data.equippedWeapon);
		// add equipped spells and their modifiers
		for (int slot = 0; slot < m_data.equippedWeaponSlots.size(); slot++) {
			eqWeapon->addSpell(slot, m_data.equippedWeaponSlots[slot].first, false);
			for (int i = 0; i < m_data.equippedWeaponSlots[slot].second.size(); ++i) {
				eqWeapon->addModifier(slot, i, m_data.equippedWeaponSlots[slot].second[i], false);
			}
		}
	}
	if (!m_data.equippedBack.empty() && g_databaseManager->itemExists(m_data.equippedBack)) {
		eqBack = new Item(m_data.equippedBack);
	}
	if (!m_data.equippedBody.empty() && g_databaseManager->itemExists(m_data.equippedBody)) {
		eqBody = new Item(m_data.equippedBody);
	}
	if (!m_data.equippedHead.empty() && g_databaseManager->itemExists(m_data.equippedHead)) {
		eqHead = new Item(m_data.equippedHead);
	}
	if (!m_data.equippedNeck.empty() && g_databaseManager->itemExists(m_data.equippedNeck)) {
		eqNeck = new Item(m_data.equippedNeck);
	}
	if (!m_data.equippedRing1.empty() && g_databaseManager->itemExists(m_data.equippedRing1)) {
		eqRing1 = new Item(m_data.equippedRing1);
	}
	if (!m_data.equippedRing2.empty() && g_databaseManager->itemExists(m_data.equippedRing2)) {
		eqRing2 = new Item(m_data.equippedRing2);
	}

	m_equippedItems.insert({ ItemType::Equipment_weapon, eqWeapon });
	m_equippedItems.insert({ ItemType::Equipment_back, eqBack });
	m_equippedItems.insert({ ItemType::Equipment_body, eqBody });
	m_equippedItems.insert({ ItemType::Equipment_head, eqHead });
	m_equippedItems.insert({ ItemType::Equipment_neck, eqNeck });
	m_equippedItems.insert({ ItemType::Equipment_ring_1, eqRing1 });
	m_equippedItems.insert({ ItemType::Equipment_ring_2, eqRing2 });
}

void CharacterCore::loadItems() {
	clearItems();
	for (auto& item : m_data.items) {
		if (!g_databaseManager->itemExists(item.first)) {
			g_logger->logError("CharacterCore", "Item not found: " + item.first);
			continue;
		}
		m_items.insert({ item.first, Item(item.first) });
	}
}

void CharacterCore::clearEquippedItems() {
	for (auto& it : m_equippedItems) {
		delete it.second;
	}
	m_equippedItems.clear();
}

void CharacterCore::clearItems() {
	m_items.clear();
}

void CharacterCore::initializeLevelMaps(const std::string& level) {
	// if these entries for the given level already exist, an insert will do nothing.
	m_data.enemiesKilled.insert({ level, std::set<int>() });
	m_data.enemiesLooted.insert({ level, std::set<int>() });
	m_data.itemsLooted.insert({ level, std::set<int>() });
	m_data.chestsLooted.insert({ level, std::set<int>() });
}

void CharacterCore::initializeMapMaps(const std::string& map) {
	// if these entries for the given map already exist, an insert will do nothing.
	m_data.waypointsUnlocked.insert({ map, std::set<int>() });
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
			if (m_data.items.find(it.first) == m_data.items.end())
				return false;
			if (m_data.items.at(it.first) < it.second)
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

bool CharacterCore::isEnemyKilled(const std::string& levelID, int objectID) {
	if (m_data.enemiesKilled.find(levelID) == m_data.enemiesKilled.end()) return false;
	if (m_data.enemiesKilled.at(levelID).find(objectID) == m_data.enemiesKilled.at(levelID).end()) return false;
	return true;
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

void CharacterCore::learnSpell(SpellID spellID) {
	SpellType type = SpellData::getSpellData(spellID).spellType;
	if (m_data.spellsLearned.find(type) == m_data.spellsLearned.end()) {
		m_data.spellsLearned.insert({type, std::set<SpellID>()});
	}
	m_data.spellsLearned.at(type).insert(spellID);
}

void CharacterCore::learnModifier(const SpellModifier& modifier) {
	if (m_data.modfiersLearned.find(modifier.type) == m_data.modfiersLearned.end()) {
		m_data.modfiersLearned.insert({ modifier.type, modifier.level});
	}
	else {
		m_data.modfiersLearned[modifier.type] = std::max(m_data.modfiersLearned[modifier.type], modifier.level);
	}
	
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

	if (!g_databaseManager->itemExists(item)) {
		g_logger->logError("CharacterCore", "Item not found: " + item);
		return;
	}
	m_items.insert({ item, Item(item) });
}

void CharacterCore::removeItem(const std::string& item, int quantity) {
	auto it = m_data.items.find(item);

	if (it != m_data.items.end()) {
		m_data.items.at(item) = m_data.items.at(item) - quantity;
		if (m_data.items.at(item) <= 0) {
			m_data.items.erase(item);
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
	if (slotNr < 0 || slotNr > m_data.equippedWeaponSlots.size() - 1) return;
	std::vector<SpellModifier>& modifiers = m_data.equippedWeaponSlots.at(slotNr).second;
	if (modifierNr < 0 || modifierNr > modifiers.size() - 1) return;

	modifiers[modifierNr].type = SpellModifierType::VOID;

	reloadWeaponSlots();
}

void CharacterCore::removeSpell(int slotNr) {
	if (slotNr < 0 || slotNr > m_data.equippedWeaponSlots.size() - 1) return;
	m_data.equippedWeaponSlots.at(slotNr).first = SpellID::VOID;
	m_data.equippedWeaponSlots.at(slotNr).second.clear();
	reloadWeaponSlots();
}

void CharacterCore::addSpell(SpellID id, int slotNr) {
	Weapon* wep = dynamic_cast<Weapon*>(m_equippedItems.at(ItemType::Equipment_weapon));
	if (wep == nullptr) return;
	if (!wep->isSpellAllowed(slotNr, id)) return;
	// check if this spell is already in another slot, if yes, remove that
	for (auto& it : m_data.equippedWeaponSlots) {
		if (it.first == id) {
			it.first = SpellID::VOID;
			it.second.clear();
		}
	}

	while (m_data.equippedWeaponSlots.size() - 1 < slotNr) {
		m_data.equippedWeaponSlots.push_back(std::pair<SpellID, std::vector<SpellModifier>>({SpellID::VOID, std::vector<SpellModifier>()}));
	}

	m_data.equippedWeaponSlots.at(slotNr).first = id;
	m_data.equippedWeaponSlots.at(slotNr).second.clear();

	reloadWeaponSlots();
}

void CharacterCore::addModifier(const SpellModifier& modifier, int slotNr, int modifierNr) {
	Weapon* wep = dynamic_cast<Weapon*>(m_equippedItems.at(ItemType::Equipment_weapon));
	if (wep == nullptr) return;
	if (wep->addModifier(slotNr, modifierNr, modifier, true)) {
		// check if this spell slot exists. If not, fill the slots
		if (m_data.equippedWeaponSlots.size() < slotNr + 1) {
			for (size_t i = m_data.equippedWeaponSlots.size(); i < slotNr + 1; ++i) {
				std::pair<SpellID, std::vector<SpellModifier>> newSlot;
				newSlot.first = SpellID::VOID;
				m_data.equippedWeaponSlots.push_back(newSlot);
			}
		}
		std::pair<SpellID, std::vector<SpellModifier>>& slot = m_data.equippedWeaponSlots.at(slotNr);
		// check if this modifier slot exist
		if (slot.second.size() < modifierNr + 1) {
			g_logger->logInfo("CharacterCore", "Adding empty modifier slots slots to the weapon!");
			for (size_t i = slot.second.size(); i < modifierNr + 1; ++i) {
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

void CharacterCore::equipItem(const std::string& item, ItemType type) {
	std::string oldItem = "";
	switch (type) {
	case ItemType::Equipment_back:
		oldItem = m_data.equippedBack;
		m_data.equippedBack = item;
		break;
	case ItemType::Equipment_body:
		oldItem = m_data.equippedBody;
		m_data.equippedBody = item;
		break;
	case ItemType::Equipment_head:
		oldItem = m_data.equippedHead;
		m_data.equippedHead = item;
		break;
	case ItemType::Equipment_neck:
		oldItem = m_data.equippedNeck;
		m_data.equippedNeck = item;
		break;
	case ItemType::Equipment_ring_1:
		oldItem = m_data.equippedRing1;
		m_data.equippedRing1 = item;
		break;
	case ItemType::Equipment_ring_2:
		oldItem = m_data.equippedRing2;
		m_data.equippedRing2 = item;
		break;
	case ItemType::Equipment_weapon:
		oldItem = m_data.equippedWeapon;
		m_data.equippedWeapon = item;
		m_data.equippedWeaponSlots.clear();
		if (!m_data.equippedWeapon.empty()) {
			std::vector<ItemWeaponSlotBean> wep = g_databaseManager->getItemWeaponSlotBeans(m_data.equippedWeapon);
			for (int i = 0; i < wep.size(); i++) {
				m_data.equippedWeaponSlots.push_back(std::pair<SpellID, std::vector<SpellModifier>>(SpellID::VOID, std::vector<SpellModifier>()));
			}
			reloadWeaponSlots();
		}
		break;
	default:
		return;
	}
	removeItem(item, 1);
	if (!oldItem.empty()) {
		addItem(oldItem, 1);
	}
	loadEquipmentItems();
	loadItems();
}