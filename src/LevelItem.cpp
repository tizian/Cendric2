#include "LevelItem.h"
#include "LevelMainCharacter.h"

void LevelItem::loadItem(LevelMainCharacter* mainChar, LevelItemID id)
{
	m_mainChar = mainChar;
	m_levelItemID = id;
}

void LevelItem::load()
{
	// nop
}

void LevelItem::onRightClick()
{
	// check if item is in range
	sf::Vector2f dist = m_mainChar->getCenter() - getCenter();
	if (sqrt(dist.x * dist.x + dist.y * dist.y) <= PICKUP_RANGE)
	{
		// pickup, create the correct item in the players inventory.
		m_mainChar->lootItem(m_itemID, 1);
		setDisposed();
	}
	else
	{
		m_screen->setTooltipText(g_textProvider->getText("OutOfRange"), sf::Vector2f(10.f, 10.f), sf::Color::Red, true);
	}
}

void LevelItem::onMouseOver()
{
	m_animatedSprite.setColor(sf::Color::Red);
	m_screen->setTooltipText(m_tooltipText, sf::Vector2f(10.f, 10.f), sf::Color::White, false);
}

void LevelItem::render(sf::RenderTarget &renderTarget)
{
	GameObject::render(renderTarget);
	m_animatedSprite.setColor(sf::Color::White);
}

GameObjectType LevelItem::getConfiguredType() const
{
	return GameObjectType::_LevelItem;
}

void LevelItem::setTooltipText(const std::wstring& tooltip)
{
	m_tooltipText = tooltip;
}

sf::Color LevelItem::getConfiguredDebugColor() const
{
	return sf::Color::Cyan;
}

void LevelItem::setItemID(ItemID id)
{
	m_itemID = id;
}
