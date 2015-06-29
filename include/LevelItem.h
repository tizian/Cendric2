#pragma once

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "TextProvider.h"
#include "Item.h"
#include "Enums/LevelItemID.h"

#include "GUI/BitmapText.h"

class LevelMainCharacter;

// A item cendric can collect in a level
class LevelItem : public GameObject
{
public:
	LevelItem() : GameObject() {}
	void load(LevelMainCharacter* mainChar, LevelItemID id);
	void onRightClick() override;
	void onMouseOver() override;
	void render(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	
	void setTooltipText(const std::wstring& tooltip);
	void setItemID(ItemID id);
	void setGoldValue(int goldValue);

	GameObjectType getConfiguredType() const override;

private:
	LevelMainCharacter* m_mainChar;
	// the item can only be picked up if the main char is in this range
	const float PICKUP_RANGE = 50.f;
	LevelItemID m_levelItemID;
	ItemID m_itemID;
	// only relevant for gold items. they have their item id void.
	int m_goldValue;
	BitmapText m_tooltipText;
	sf::Time m_tooltipTime = sf::Time::Zero;
};