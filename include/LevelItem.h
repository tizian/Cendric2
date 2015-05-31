#pragma once

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "TextProvider.h"
#include "Item.h"
#include "Enums/LevelItemID.h"

class LevelMainCharacter;

// A item cendric can collect in a level
class LevelItem : public GameObject
{
public:
	void loadItem(LevelMainCharacter* mainChar, LevelItemID id);
	void load() override;
	void onRightClick() override;
	void onMouseOver() override;
	void render(sf::RenderTarget& renderTarget) override;
	
	void setTooltipText(const std::string& tooltip);
	void setItemID(ItemID id);

	GameObjectType getConfiguredType() const override;
	sf::Color getConfiguredDebugColor() const override;

private:
	LevelMainCharacter* m_mainChar;
	// the item can only be picked up if the main char is in this range
	const float PICKUP_RANGE = 50.f;
	LevelItemID m_levelItemID;
	ItemID m_itemID;
	std::string m_tooltipText;
};