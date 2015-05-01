#include "LevelItem.h"
#include "MainCharacter.h"

void LevelItem::loadItem(MainCharacter* mainChar)
{
	m_mainChar = mainChar;
	load();
}

void LevelItem::onRightClick()
{
	// TODO pickup
	setDisposed();
}

void LevelItem::onMouseOver()
{
	m_animatedSprite.setColor(sf::Color::Red);
	m_screen->setTooltipText(getConfiguredTooltipText(), sf::Vector2f(10.f, 10.f), sf::Color::White);
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
