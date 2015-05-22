#include "Item.h"

Item::Item(ItemBean& bean)
{
	m_bean = bean;
}

Texts Item::getDescription() const
{
	return m_bean.description;
}

ItemID Item::getID() const
{
	return m_bean.id;
}

ItemType Item::getType() const
{
	return m_bean.type;
}

LevelEquipmentID Item::getLevelEquipmentID() const
{
	return m_bean.levelEquipment;
}

ResourceID Item::getIcon() const
{
	return m_bean.icon;
}

int Item::getValue() const
{
	return m_bean.value;
}

const AttributeBean& Item::getAttributes() const
{
	return m_bean.attributes;
}