#include "CharacterCore.h"
#include "Item.h"

using namespace std;

const char* CharacterCore::QUICKSAVE_LOCATION = "saves/quicksave.sav";
const char* CharacterCore::DEBUGSAVE_LOCATION = "saves/debug.sav";

CharacterCore::CharacterCore()
{
	m_data = DEFAULT_CORE;
}

CharacterCore::CharacterCore(const CharacterCoreData& data)
{
	m_data = data;
	m_stopwatch.restart();
	reloadAttributes();
	loadQuests();
}

CharacterCore::~CharacterCore()
{
	clearEquippedItems();
	clearItems();
}

bool CharacterCore::quickload()
{
	return load(QUICKSAVE_LOCATION);
}

bool CharacterCore::load(const std::string& fileName)
{
	CharacterCoreReader reader;
	
	if (!reader.readCharacterCore(fileName, m_data))
	{
		return false;
	} 
	
	// measuring the time played with this save.
	m_stopwatch.restart();
	reloadAttributes();
	loadQuests();
	return true;
}

void CharacterCore::loadQuests()
{
	m_quests.clear();
	QuestLoader loader;
	for (auto& it : m_data.questStates)
	{
		QuestData data = loader.loadQuest(it.first);
		if (data.id.empty())
		{
			g_logger->logError("CharacterCore::loadQuests", "Could not load quest: " + it.first);
			continue;
		}
		m_quests.insert({ data.id, data });
	}
}

void CharacterCore::loadNew()
{
	// start map & position when a new game is loaded
	m_data.isInLevel = false;
	m_data.currentMap = "res/map/firstmap/firstmap.tmx";
	m_data.currentMapPosition = sf::Vector2f(4400.0f, 650.0f); 
	m_data.attributes.currentHealthPoints = 100;
	m_data.attributes.maxHealthPoints = 100;
	m_data.attributes.critical = 5;
	m_stopwatch.restart();
	reloadAttributes();
}

const Item* CharacterCore::getEquippedItem(ItemType type)
{
	if (m_equippedItems.empty())
	{
		loadEquipmentItems();
	}
	return m_equippedItems.at(type);
}

const Item& CharacterCore::getItem(const std::string& id)
{
	if (m_items.empty())
	{
		loadItems();
	}
	return m_items.at(id);
}

const Weapon* CharacterCore::getWeapon()
{
	const Weapon* weapon;
	if (!(weapon = dynamic_cast<const Weapon*>(getEquippedItem(ItemType::Equipment_weapon))))
	{
		// unexpected
		return nullptr;
	}
	return weapon;
}

NPCState CharacterCore::getNPCState(const std::string& id) const
{
	if (m_data.npcStates.find(id) != m_data.npcStates.end())
	{
		return m_data.npcStates.at(id);
	}
	return NPCState::Never_talked;
}

QuestState CharacterCore::getQuestState(const std::string& id) const
{
	if (m_data.questStates.find(id) != m_data.questStates.end())
	{
		return m_data.questStates.at(id);
	}
	return QuestState::VOID;
}

void CharacterCore::setNPCState(const std::string& id, NPCState state)
{
	if (m_data.npcStates.find(id) != m_data.npcStates.end())
	{
		m_data.npcStates[id] = state;
		return;
	}
	m_data.npcStates.insert({ id, state });
}

void CharacterCore::setQuestState(const std::string& id, QuestState state)
{
	if (state == QuestState::Started && m_data.questStates.find(id) == m_data.questStates.end())
	{
		QuestLoader loader;
		QuestData newQuest = loader.loadQuest(id);
		if (newQuest.id.empty())
		{
			g_logger->logError("CharacterCore", "Could not load quest: " + id);
			return;
		}
		m_quests.insert({ newQuest.id, newQuest });
		m_data.questStates.insert({ id, state });
		return;
	}
	if (state != QuestState::Started && m_data.questStates.find(id) != m_data.questStates.end())
	{
		m_data.questStates[id] = state;
		return;
	}
	g_logger->logError("CharacterCore", "Cannot change quest state for quest: " + id + ". Either the quest has already started (and cannot be started again) or the quest has not yet started and needs to be started first.");
}

void CharacterCore::setQuickslot(const std::string& item, int nr)
{
	if (nr == 1)
	{
		m_data.quickSlot1 = item;
	}
	else if (nr == 2)
	{
		m_data.quickSlot2 = item;
	}
}

bool CharacterCore::save(const std::string& fileName, const string& name) 
{
	m_data.timePlayed += m_stopwatch.restart();
	m_data.dateSaved = time(nullptr);
	m_data.saveGameName = name;
	
	// write to savefile.
	CharacterCoreWriter writer;

	writer.createFile(fileName);
	return writer.saveToFile(fileName, m_data);
}

bool CharacterCore::quicksave()
{
	m_data.timePlayed += m_stopwatch.restart();
	m_data.dateSaved = time(nullptr);
	m_data.saveGameName = "Quicksave";

	// write to savefile.
	CharacterCoreWriter writer;
	writer.createFile(QUICKSAVE_LOCATION);
	return writer.saveToFile(QUICKSAVE_LOCATION, m_data);
}

bool CharacterCore::createFile(const std::string& fileName) const
{
	CharacterCoreWriter writer;
	return writer.createFile(fileName);
}

void CharacterCore::reloadAttributes()
{
	m_totalAttributes = m_data.attributes;
	loadEquipmentItems();
	for (auto &it : m_equippedItems)
	{
		if (it.second == nullptr) continue;
		m_totalAttributes.addBean(it.second->getAttributes());
	}
	m_totalAttributes.currentHealthPoints = m_totalAttributes.maxHealthPoints;
}

void CharacterCore::reloadWeaponSlots()
{
	Weapon* wep = dynamic_cast<Weapon*>(m_equippedItems.at(ItemType::Equipment_weapon));
	if (wep == nullptr) return;
	wep->reload();
	for (int slot = 0; slot < m_data.equippedWeaponSlots.size(); slot++)
	{
		wep->addSpell(slot, m_data.equippedWeaponSlots[slot].first, false);
		for (auto& it : m_data.equippedWeaponSlots[slot].second)
		{
			wep->addModifier(slot, it, false);
		}
	}
}

void CharacterCore::loadEquipmentItems()
{
	clearEquippedItems();

	Weapon* eqWeapon = nullptr;
	Item* eqBack = nullptr;
	Item* eqBody = nullptr;
	Item* eqHead = nullptr;
	Item* eqNeck = nullptr;
	Item* eqRing1 = nullptr;
	Item* eqRing2 = nullptr;
	if (!m_data.equippedWeapon.empty() && g_resourceManager->getItemBean(m_data.equippedWeapon) != nullptr)
	{
		eqWeapon = new Weapon(*g_resourceManager->getItemBean(m_data.equippedWeapon));
		// add equipped spells and their modifiers
		for (int slot = 0; slot < m_data.equippedWeaponSlots.size(); slot++)
		{
			eqWeapon->addSpell(slot, m_data.equippedWeaponSlots[slot].first, false);
			for (auto& it : m_data.equippedWeaponSlots[slot].second)
			{
				eqWeapon->addModifier(slot, it, false);
			}
		}
	}
	if (!m_data.equippedBack.empty() && g_resourceManager->getItemBean(m_data.equippedBack) != nullptr)
	{
		eqBack = new Item(*g_resourceManager->getItemBean(m_data.equippedBack));
	}
	if (!m_data.equippedBody.empty() && g_resourceManager->getItemBean(m_data.equippedBody) != nullptr)
	{
		eqBody = new Item(*g_resourceManager->getItemBean(m_data.equippedBody));
	}
	if (!m_data.equippedHead.empty() && g_resourceManager->getItemBean(m_data.equippedHead) != nullptr)
	{
		eqHead = new Item(*g_resourceManager->getItemBean(m_data.equippedHead));
	}
	if (!m_data.equippedNeck.empty() && g_resourceManager->getItemBean(m_data.equippedNeck) != nullptr)
	{
		eqNeck = new Item(*g_resourceManager->getItemBean(m_data.equippedNeck));
	}
	if (!m_data.equippedRing1.empty() && g_resourceManager->getItemBean(m_data.equippedRing1) != nullptr)
	{
		eqRing1 = new Item(*g_resourceManager->getItemBean(m_data.equippedRing1));
	}
	if (!m_data.equippedRing2.empty() && g_resourceManager->getItemBean(m_data.equippedRing2) != nullptr)
	{
		eqRing2 = new Item(*g_resourceManager->getItemBean(m_data.equippedRing2));
	}
	
	m_equippedItems.insert({ ItemType::Equipment_weapon, eqWeapon });
	m_equippedItems.insert({ ItemType::Equipment_back, eqBack });
	m_equippedItems.insert({ ItemType::Equipment_body, eqBody });
	m_equippedItems.insert({ ItemType::Equipment_head, eqHead });
	m_equippedItems.insert({ ItemType::Equipment_neck, eqNeck });
	m_equippedItems.insert({ ItemType::Equipment_ring_1, eqRing1 });
	m_equippedItems.insert({ ItemType::Equipment_ring_2, eqRing2 });
}

void CharacterCore::loadItems()
{
	clearItems();
	for (auto& it : m_data.items)
	{
		const ItemBean* bean = g_resourceManager->getItemBean(it.first);
		if (bean == nullptr) continue;
		m_items.insert({ it.first, Item(*bean) });
	}
}

void CharacterCore::clearEquippedItems()
{
	for (auto& it : m_equippedItems)
	{
		delete it.second;
	}
	m_equippedItems.clear();
}

void CharacterCore::clearItems()
{
	m_items.clear();
}

void CharacterCore::initializeMaps(const std::string& level)
{
	// if these entries for the given level already exist, an insert will do nothing.
	m_data.enemiesKilled.insert({ level, std::set<int>() });
	m_data.enemiesLooted.insert({ level, std::set<int>() });
	m_data.itemsLooted.insert({ level, std::set<int>() });
	m_data.chestsLooted.insert({ level, std::set<int>() });
}

void CharacterCore::setEnemyKilled(const std::string& level, int pos)
{
	m_data.enemiesKilled.at(level).insert(pos);
}

void CharacterCore::setEnemyLooted(const std::string& level, int pos)
{
	m_data.enemiesLooted.at(level).insert(pos);
}

void CharacterCore::setItemLooted(const std::string& level, int pos)
{
	m_data.itemsLooted.at(level).insert(pos);
}

void CharacterCore::setChestLooted(const std::string& level, int pos)
{
	m_data.chestsLooted.at(level).insert(pos);
}

const QuestData* CharacterCore::getQuestData(const std::string& questID) const
{
	if (m_quests.find(questID) == m_quests.end())
	{
		g_logger->logError("CharacterCore", "Quest: " + questID + " has no quest data!");
		return nullptr;
	}
	return &m_quests.at(questID);
}

bool CharacterCore::isQuestComplete(const std::string& questID) const
{
	if (getQuestState(questID) != QuestState::Started) return false;
	if (m_quests.find(questID) == m_quests.end())
	{
		g_logger->logError("CharacterCore", "Quest: " + questID + " has no quest data!");
		return false;
	}
	QuestData data = m_quests.at(questID);

	// check quest conditions
	if (!data.conditions.empty())
	{
		if (m_data.questConditionProgress.find(questID) == m_data.questConditionProgress.end()) 
			return false;
		for (auto& it : data.conditions)
		{
			if (m_data.questConditionProgress.at(questID).find(it) == m_data.questConditionProgress.at(questID).end())
				return false;
		}
	}

	// check quest targets
	if (!data.targets.empty())
	{
		if (m_data.questTargetProgress.find(questID) == m_data.questTargetProgress.end())
			return false;
		for (auto& it : data.targets)
		{
			if (m_data.questTargetProgress.at(questID).find(it.first) == m_data.questTargetProgress.at(questID).end())
				return false;
			if (m_data.questTargetProgress.at(questID).at(it.first) < it.second)
				return false;
		}
	}
	
	// check collectibles
	if (!data.collectibles.empty())
	{
		for (auto& it : data.collectibles)
		{
			if (m_data.items.find(it.first) == m_data.items.end())
				return false;
			if (m_data.items.at(it.first) < it.second)
				return false;
		}
	}

	return true;
}

void CharacterCore::setQuestTargetKilled(const std::pair<std::string, std::string>& questtarget)
{
	std::string questID = questtarget.first;
	std::string name = questtarget.second;
	if (m_data.questTargetProgress.find(questID) == m_data.questTargetProgress.end())
	{
		m_data.questTargetProgress.insert({ questID, std::map<std::string, int>() });
	}
	if (m_data.questTargetProgress.at(questID).find(name) == m_data.questTargetProgress.at(questID).end())
	{
		m_data.questTargetProgress.at(questID).insert({ name, 0 });
	}
	m_data.questTargetProgress.at(questID).at(name) = m_data.questTargetProgress.at(questID).at(name) + 1;
}

void CharacterCore::setQuestConditionFulfilled(const std::string& questID, const std::string& condition)
{
	if (m_data.questConditionProgress.find(questID) == m_data.questConditionProgress.end())
	{
		m_data.questConditionProgress.insert({ questID, std::set<std::string>()});
	}
	m_data.questConditionProgress.at(questID).insert(condition);
}

const CharacterCoreData& CharacterCore::getData() const
{
	return m_data;
}

const AttributeBean& CharacterCore::getTotalAttributes() const
{
	return m_totalAttributes;
}

std::map<std::string, int>* CharacterCore::getItems()
{
	return &(m_data.items);
}

void CharacterCore::addGold(int gold)
{
	m_data.gold += std::max(gold, 0);
}

void CharacterCore::removeGold(int gold)
{
	m_data.gold -= std::min(m_data.gold, gold);
}

void CharacterCore::addItem(const std::string& item, int quantity)
{
	auto it = m_data.items.find(item);

	if (it != m_data.items.end())
	{
		m_data.items.at(item) = m_data.items.at(item) + quantity;
	}
	else
	{
		m_data.items.insert({ item, quantity });
	}
}

void CharacterCore::removeItem(const std::string& item, int quantity)
{
	auto it = m_data.items.find(item);

	if (it != m_data.items.end())
	{
		m_data.items.at(item) = m_data.items.at(item) - quantity;
		if (m_data.items.at(item) <= 0)
		{
			m_data.items.erase(item);
		}
	}
}

void CharacterCore::setMap(const sf::Vector2f& position, const std::string& map)
{
	m_data.currentMap = map;
	m_data.currentMapPosition = position;
	m_data.isInLevel = false;
}

void CharacterCore::setLevel(const sf::Vector2f& position, const std::string& level)
{
	m_data.currentLevel = level;
	m_data.currentLevelPosition = position;
	m_data.isInLevel = true;
}

void CharacterCore::removeModifier(SpellModifierType type, int slotNr)
{
	if (slotNr < 0 || slotNr > m_data.equippedWeaponSlots.size() - 1) return;

	std::vector<SpellModifier>& modifiers = m_data.equippedWeaponSlots.at(slotNr).second;
	for (auto& it = modifiers.begin(); it != modifiers.end(); /* don't increment here */)
	{
		if (it->type == type)
		{
			it = modifiers.erase(it);
		}
		else ++it;
	}
	reloadWeaponSlots();
}

void CharacterCore::removeSpell(int slotNr)
{
	if (slotNr < 0 || slotNr > m_data.equippedWeaponSlots.size() - 1) return;
	m_data.equippedWeaponSlots.at(slotNr).first = SpellID::VOID;
	m_data.equippedWeaponSlots.at(slotNr).second.clear();
	reloadWeaponSlots();
}

void CharacterCore::addSpell(SpellID id, int slotNr)
{
	Weapon* wep = dynamic_cast<Weapon*>(m_equippedItems.at(ItemType::Equipment_weapon));
	if (wep == nullptr) return;
	if (!wep->isSpellAllowed(slotNr, id)) return;
	// check if this spell is already in another slot, if yes, remove that
	for (auto& it : m_data.equippedWeaponSlots)
	{
		if (it.first == id)
		{
			it.first = SpellID::VOID;
			it.second.clear();
		}
	}
	m_data.equippedWeaponSlots.at(slotNr).first = id;
	m_data.equippedWeaponSlots.at(slotNr).second.clear();

	reloadWeaponSlots();
}

void CharacterCore::addModifier(const SpellModifier& modifier, int slotNr)
{
	Weapon* wep = dynamic_cast<Weapon*>(m_equippedItems.at(ItemType::Equipment_weapon));
	if (wep == nullptr) return;
	if (wep->addModifier(slotNr, modifier, true))
	{
		std::pair<SpellID, std::vector<SpellModifier>>& slot = m_data.equippedWeaponSlots.at(slotNr);
		// check if this type already exists. if yes, remove it.
		for (auto& it = slot.second.begin(); it != slot.second.end(); /* don't increment here */)
		{
			if (it->type == modifier.type)
			{
				it = slot.second.erase(it);
			}
			else ++it;
		}
		
		slot.second.push_back(modifier);
	}
	reloadWeaponSlots();
}

void CharacterCore::equipItem(const std::string& item, ItemType type)
{
	std::string oldItem = "";
	switch (type)
	{
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
		for (int i = 0; i < (*(g_resourceManager->getItemBean(m_data.equippedWeapon))).weaponSlots.size(); i++)
		{
			m_data.equippedWeaponSlots.push_back(std::pair<SpellID, std::vector<SpellModifier>>(SpellID::VOID, std::vector<SpellModifier>()));
		}
		break;
	default: 
		return;
	}
	removeItem(item, 1);
	if (!oldItem.empty())
	{
		addItem(oldItem, 1);
	}
	loadEquipmentItems();
	loadItems();
}