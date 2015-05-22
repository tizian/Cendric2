#include "CharacterCore.h"

using namespace std;

CharacterCore::CharacterCore()
{
	m_data = DEFAULT_CORE;
}

CharacterCore::~CharacterCore()
{
}

bool CharacterCore::load(char* fileName)
{
	CharacterCoreReader reader;
	
	if (!reader.readCharacterCore(fileName, m_data))
	{
		return false;
	}
	
	m_currentHealthPoints = m_data.attributes.maxHealthPoints;
	m_currentManaPoints = m_data.attributes.maxManaPoints;
	reloadStats();
	return true;
}

const Item& CharacterCore::getEquippedItem(ItemType type)
{
	if (m_equippedItems.empty())
	{
		loadEquipmentItems();
	}
	return m_equippedItems.at(type);
}

void CharacterCore::save(char* fileName) const
{

}

void CharacterCore::reloadStats()
{
	m_totalAttributes = m_data.attributes;
	if(m_equippedItems.empty())
	{
		loadEquipmentItems();
	}
	for (auto &it : m_equippedItems)
	{
		addBean(m_totalAttributes, it.second.getAttributes());
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
	m_items.clear();
}

void CharacterCore::clearItems()
{
	m_equippedItems.clear();
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
	firstBean.damageFire += secondBean.damageFire;
}