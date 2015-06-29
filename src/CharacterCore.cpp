#include "CharacterCore.h"

using namespace std;

CharacterCore::CharacterCore()
{
	m_data = DEFAULT_CORE;
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
	reloadStats();
	return true;
}

void CharacterCore::loadNew()
{
	// start map & position when a new game is loaded
	m_data.playerName = "Dummy";
	m_data.currentMap = MapID::Firstmap; 
	m_data.currentMapPosition = sf::Vector2f(4400.0f, 650.0f); 
	m_data.attributes.currentHealthPoints = 100;
	m_data.attributes.currentManaPoints = 100;
	m_data.attributes.maxHealthPoints = 100;
	m_data.attributes.maxManaPoints = 100;
	m_data.attributes.healthRegenerationPerS = 0;
	m_data.attributes.manaRegenerationPerS = 0;
	m_stopwatch.restart();
	reloadStats();
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

void CharacterCore::reloadStats()
{
	m_totalAttributes = m_data.attributes;
	loadEquipmentItems();
	for (auto &it : m_equippedItems)
	{
		addBean(m_totalAttributes, it.second.getAttributes());
	}
	if (m_totalAttributes.currentHealthPoints > m_totalAttributes.maxHealthPoints)
	{
		m_totalAttributes.currentHealthPoints = m_totalAttributes.maxHealthPoints;
	}
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
	reloadStats();
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
	firstBean.resistanceFire += secondBean.resistanceFire;
	firstBean.resistanceIce += secondBean.resistanceIce;
	firstBean.resistancePhysical += secondBean.resistancePhysical;
	firstBean.maxHealthPoints += secondBean.maxHealthPoints;
	firstBean.maxManaPoints += secondBean.maxManaPoints;
	firstBean.currentHealthPoints = (firstBean.maxHealthPoints < firstBean.currentHealthPoints + secondBean.currentHealthPoints) ? firstBean.maxHealthPoints : (firstBean.currentHealthPoints + secondBean.currentHealthPoints);
	firstBean.currentManaPoints = (firstBean.maxManaPoints < firstBean.currentManaPoints + secondBean.currentManaPoints) ? firstBean.maxManaPoints : (firstBean.currentManaPoints + secondBean.currentManaPoints);
}