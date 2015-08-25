#include "Item.h"

Item::Item(const ItemBean& bean)
{
	m_bean = bean;
}

Item::~Item()
{
}

const std::string& Item::getDescription() const
{
	return m_bean.description;
}

const std::string& Item::getID() const
{
	return m_bean.id;
}

ItemType Item::getType() const
{
	return m_bean.type;
}

const sf::Vector2i& Item::getIconTextureLocation() const
{
	return m_bean.iconTextureLocation;
}

int Item::getValue() const
{
	return m_bean.goldValue;
}

const AttributeBean& Item::getAttributes() const
{
	return m_bean.attributes;
}

const ItemBean& Item::getBean() const
{
	return m_bean;
}