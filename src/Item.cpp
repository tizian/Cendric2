#include "Item.h"
#include "MainCharacter.h"

void Item::loadItem(MainCharacter* mainChar)
{
	m_mainChar = mainChar;
	load();
}

void Item::onRightClick()
{
	// TODO pickup
}

void Item::onMouseOver()
{
	m_animatedSprite.setColor(sf::Color::Red);
}

void Item::render(sf::RenderTarget &renderTarget)
{
	GameObject::render(renderTarget);
	m_animatedSprite.setColor(sf::Color::White);
}

GameObjectType Item::getConfiguredType() const
{
	return GameObjectType::_Item;
}
