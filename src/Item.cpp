#include "Item.h"

Item::Item(ItemBean& bean)
{
	m_bean.description = bean.description;
	m_bean.icon = bean.icon;
	m_bean.id = bean.id;
	m_bean.type = bean.type;
	m_bean.value = bean.value;
}

const std::string& Item::getDescription() const
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

ResourceID Item::getIcon() const
{
	return m_bean.icon;
}

int Item::getValue() const
{
	return m_bean.value;
}