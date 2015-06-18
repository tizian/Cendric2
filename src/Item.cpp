#include "Item.h"

Item::Item(ItemBean& bean)
{
	m_bean = bean;
}

const std::string& Item::getDescription() const
{
	return m_bean.description;
}

const std::string& Item::getName() const
{
	return m_bean.name;
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

const sf::Vector2i& Item::getIconTextureLocation() const
{
	return m_bean.iconTextureLocation;
}

int Item::getValue() const
{
	return m_bean.value;
}

const AttributeBean& Item::getAttributes() const
{
	return m_bean.attributes;
}