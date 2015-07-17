#include "CharacterCore.h"

using namespace std;

CharacterCore::CharacterCore()
{
	m_data = DEFAULT_CORE;
}

void CharacterCore::update(const sf::Time& frameTime)
{
	if (m_foodAttributes.first > sf::Time::Zero)
	{
		m_foodAttributes.first -= frameTime;
		if (m_foodAttributes.first <= sf::Time::Zero)
		{
			reloadAttributes();
		}
	}
}

CharacterCore::~CharacterCore()
{
	clearEquippedItems();
	clearItems();
}

bool CharacterCore::load(const char* fileName)
{
	CharacterCoreReader reader;
	
	if (!reader.readCharacterCore(fileName, m_data))
	{
		return false;
	} 
	
	// measuring the time played with this save.
	m_stopwatch.restart();
	reloadAttributes();
	return true;
}

void CharacterCore::loadNew()
{
	// start map & position when a new game is loaded
	m_data.currentMap = MapID::Firstmap; 
	m_data.currentMapPosition = sf::Vector2f(4400.0f, 650.0f); 
	m_data.attributes.currentHealthPoints = 100;
	m_data.attributes.maxHealthPoints = 100;
	m_data.attributes.critical = 5;
	m_stopwatch.restart();
	reloadAttributes();
}

const Item& CharacterCore::getEquippedItem(ItemType type)
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

NPCState CharacterCore::getNPCState(NPCID id)
{
	if (m_data.npcStates.find(id) != m_data.npcStates.end())
	{
		return m_data.npcStates[id];
	}
	return NPCState::Never_talked;
}

QuestState CharacterCore::getQuestState(QuestID id)
{
	if (m_data.questStates.find(id) != m_data.questStates.end())
	{
		return m_data.questStates[id];
	}
	return QuestState::Void;
}

void CharacterCore::setNPCState(NPCID id, NPCState state)
{
	if (m_data.npcStates.find(id) != m_data.npcStates.end())
	{
		m_data.npcStates[id] = state;
		return;
	}
	m_data.npcStates.insert({ id, state });
}

void CharacterCore::setQuestState(QuestID id, QuestState state)
{
	if (m_data.questStates.find(id) != m_data.questStates.end())
	{
		m_data.questStates[id] = state;
		return;
	}
	m_data.questStates.insert({ id, state });
}

bool CharacterCore::save(const char* fileName) 
{
	m_data.timePlayed += m_stopwatch.restart();
	
	// write to savefile.
	CharacterCoreWriter writer;
	writer.createFile(fileName);
	return writer.saveToFile(fileName, m_data);
}

bool CharacterCore::createFile(const char* fileName) const
{
	CharacterCoreWriter writer;
	return writer.createFile(fileName);
}

void CharacterCore::reloadAttributes()
{
	m_totalAttributes = m_data.attributes;
	m_foodAttributes.first = sf::Time::Zero;
	m_foodAttributes.second = ZERO_ATTRIBUTES;
	loadEquipmentItems();
	for (auto &it : m_equippedItems)
	{
		m_totalAttributes.addBean(it.second.getAttributes());
	}
	if (m_totalAttributes.currentHealthPoints > m_totalAttributes.maxHealthPoints)
	{
		m_totalAttributes.currentHealthPoints = m_totalAttributes.maxHealthPoints;
	}
	m_totalAttributes.calculateAttributes();
}

void CharacterCore::consumeFood(sf::Time& duration, AttributeBean& attributes)
{
	reloadAttributes();
	m_foodAttributes = pair<sf::Time, AttributeBean>(duration, attributes);
	m_totalAttributes.addBean(attributes);
	m_totalAttributes.calculateAttributes();
}

void CharacterCore::loadEquipmentItems()
{
	clearEquippedItems();

	Item eqWeapon(DEFAULT_ITEM);
	Item eqBack(DEFAULT_ITEM);
	Item eqBody(DEFAULT_ITEM);
	Item eqHead(DEFAULT_ITEM);
	Item eqNeck(DEFAULT_ITEM);
	Item eqRing1(DEFAULT_ITEM);
	Item eqRing2(DEFAULT_ITEM);
	if (!m_data.equippedWeapon.empty() && g_resourceManager->getItemBean(m_data.equippedWeapon) != nullptr)
	{
		eqWeapon = Item(*g_resourceManager->getItemBean(m_data.equippedWeapon));
	}
	if (!m_data.equippedBack.empty() && g_resourceManager->getItemBean(m_data.equippedBack) != nullptr)
	{
		eqBack = Item(*g_resourceManager->getItemBean(m_data.equippedBack));
	}
	if (!m_data.equippedBody.empty() && g_resourceManager->getItemBean(m_data.equippedBody) != nullptr)
	{
		eqBody = Item(*g_resourceManager->getItemBean(m_data.equippedBody));
	}
	if (!m_data.equippedHead.empty() && g_resourceManager->getItemBean(m_data.equippedHead) != nullptr)
	{
		eqHead = Item(*g_resourceManager->getItemBean(m_data.equippedHead));
	}
	if (!m_data.equippedNeck.empty() && g_resourceManager->getItemBean(m_data.equippedNeck) != nullptr)
	{
		eqNeck = Item(*g_resourceManager->getItemBean(m_data.equippedNeck));
	}
	if (!m_data.equippedRing1.empty() && g_resourceManager->getItemBean(m_data.equippedRing1) != nullptr)
	{
		eqRing1 = Item(*g_resourceManager->getItemBean(m_data.equippedRing1));
	}
	if (!m_data.equippedRing2.empty() && g_resourceManager->getItemBean(m_data.equippedRing2) != nullptr)
	{
		eqRing2 = Item(*g_resourceManager->getItemBean(m_data.equippedRing2));
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
	for (auto &it : m_data.items)
	{
		m_items.insert({ it.first, Item(*g_resourceManager->getItemBean(it.first)) });
	}
}

void CharacterCore::clearEquippedItems()
{
	m_equippedItems.clear();
}

void CharacterCore::clearItems()
{
	m_items.clear();
}

const CharacterCoreData& CharacterCore::getData() const
{
	return m_data;
}
AttributeBean* CharacterCore::getTotalAttributes()
{
	return &m_totalAttributes;
}

std::map<std::string, int>* CharacterCore::getItems()
{
	return &(m_data.items);
}

void CharacterCore::addGold(int gold)
{
	m_data.gold += std::max(gold, 0);
}

void CharacterCore::resetHealth()
{
	m_data.attributes.currentHealthPoints = m_data.attributes.maxHealthPoints;
	reloadAttributes();
}

void CharacterCore::setMap(const sf::Vector2f& position, MapID map)
{
	m_data.currentMap = map;
	m_data.currentMapPosition = position;
}

void CharacterCore::setLevel(const sf::Vector2f& position, LevelID level)
{
	m_data.currentLevel = level;
	m_data.currentLevelPosition = position;
}