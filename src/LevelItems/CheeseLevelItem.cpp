#include "LevelItems\CheeseLevelItem.h"

void CheeseLevelItem::load()
{
	setSpriteOffset(sf::Vector2f(-10.f, -10.f));
	setBoundingBox(sf::FloatRect(0, 0, 30, 30));

	Animation idleAnimation;
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_items_food));
	idleAnimation.addFrame(sf::IntRect(0, 0, 50, 50));

	addAnimation(GameObjectState::Idle, idleAnimation);

	setFrameTime(sf::seconds(0.1f));

	// initial values
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
}

LevelItemID CheeseLevelItem::getConfiguredItemID() const
{
	return LevelItemID::Food_Cheese;
}

const std::string& CheeseLevelItem::getConfiguredTooltipText() const
{
	return g_textProvider->getText(Texts::Levelitem_tooltip_cheese);
}