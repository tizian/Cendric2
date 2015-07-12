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

const Item& CharacterCore::getItem(ItemID id)
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
		addBean(m_totalAttributes, it.second.getAttributes());
	}
	if (m_totalAttributes.currentHealthPoints > m_totalAttributes.maxHealthPoints)
	{
		m_totalAttributes.currentHealthPoints = m_totalAttributes.maxHealthPoints;
	}
	calculateAttributes(m_totalAttributes);
}

void CharacterCore::consumeFood(sf::Time& duration, AttributeBean& attributes)
{
	reloadAttributes();
	m_foodAttributes = pair<sf::Time, AttributeBean>(duration, attributes);
	addBean(m_totalAttributes, attributes);
	calculateAttributes(m_totalAttributes);
}

void CharacterCore::calculateAttributes(AttributeBean& bean)
{
	bean.criticalHitChance = max(0, min(60, bean.critical));
	bean.cooldownMultiplier = 1.f / (1.f + (bean.haste / 100.f));

	bean.physicalMultiplier = calculateDamageReduction(bean.resistancePhysical);
	bean.fireMultiplier = calculateDamageReduction(bean.resistanceFire);
	bean.iceMultiplier = calculateDamageReduction(bean.resistanceIce);
	bean.shadowMultiplier = calculateDamageReduction(bean.resistanceShadow);
	bean.lightMultiplier = calculateDamageReduction(bean.resistanceLight);
}

float CharacterCore::calculateDamageReduction(int resistance) const
{
	if (resistance >= 0)
	{
		return 100.f / (100.f + resistance);
	}
	return 2.f - 100.f / (100.f - resistance);
}

void CharacterCore::loadEquipmentItems()
{
	clearEquippedItems();
	ItemFactory factory;

	ItemBean newItem = DEFAULT_ITEM;
	factory.loadItemBean(newItem, m_data.equippedWeapon);
	m_equippedItems.insert({ ItemType::Equipment_weapon , Item(newItem)});

	newItem = DEFAULT_ITEM;
	factory.loadItemBean(newItem, m_data.equippedBack);
	m_equippedItems.insert({ ItemType::Equipment_back, Item(newItem) });

	newItem = DEFAULT_ITEM;
	factory.loadItemBean(newItem, m_data.equippedBody);
	m_equippedItems.insert({ ItemType::Equipment_body, Item(newItem) });

	newItem = DEFAULT_ITEM;
	factory.loadItemBean(newItem, m_data.equippedHead);
	m_equippedItems.insert({ ItemType::Equipment_head, Item(newItem) });

	newItem = DEFAULT_ITEM;
	factory.loadItemBean(newItem, m_data.equippedNeck);
	m_equippedItems.insert({ ItemType::Equipment_neck, Item(newItem) });

	newItem = DEFAULT_ITEM;
	factory.loadItemBean(newItem, m_data.equippedRing1);
	m_equippedItems.insert({ ItemType::Equipment_ring_1, Item(newItem) });

	newItem = DEFAULT_ITEM;
	factory.loadItemBean(newItem, m_data.equippedRing2);
	m_equippedItems.insert({ ItemType::Equipment_ring_2, Item(newItem) });
}

void CharacterCore::loadItems()
{
	clearItems();
	ItemBean newItem;
	ItemFactory factory;
	for (auto &it : m_data.items)
	{
		newItem = DEFAULT_ITEM;
		factory.loadItemBean(newItem, it.first);
		m_items.insert({ it.first, Item(newItem) });
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

std::map<ItemID, int>* CharacterCore::getItems()
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

void CharacterCore::addBean(AttributeBean& firstBean, const AttributeBean& secondBean) const
{
	firstBean.damageFire += secondBean.damageFire;
	firstBean.damageIce += secondBean.damageIce;
	firstBean.damagePhysical += secondBean.damagePhysical;
	firstBean.damageLight += secondBean.damageLight;
	firstBean.damageShadow += secondBean.damageShadow;
	firstBean.resistanceFire += secondBean.resistanceFire;
	firstBean.resistanceIce += secondBean.resistanceIce;
	firstBean.resistancePhysical += secondBean.resistancePhysical;
	firstBean.resistanceLight += secondBean.resistanceLight;
	firstBean.resistanceShadow += secondBean.resistanceShadow;
	firstBean.maxHealthPoints += secondBean.maxHealthPoints;
	firstBean.haste += secondBean.haste;
	firstBean.critical += secondBean.critical;
	firstBean.healthRegenerationPerS += secondBean.healthRegenerationPerS;
	firstBean.currentHealthPoints = (firstBean.maxHealthPoints < firstBean.currentHealthPoints + secondBean.currentHealthPoints) ? firstBean.maxHealthPoints : (firstBean.currentHealthPoints + secondBean.currentHealthPoints);
}