#pragma once

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "TextProvider.h"
#include "Item.h"

#include "GUI/BitmapText.h"
#include "Enums/ItemType.h"

class LevelMainCharacter;

// A item cendric can collect in a level
class LevelItem : public GameObject
{
public:
	LevelItem() : GameObject() {}
	void load(LevelMainCharacter* mainChar, const ItemBean& bean, const sf::Vector2f& position);
	void onRightClick() override;
	void onInteractKey() override;
	void onMouseOver() override;
	void render(sf::RenderTarget& renderTarget) override;
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;
	
	GameObjectType getConfiguredType() const override;

	void setTooltipText(const std::wstring& tooltip);
	void setSpawnPosition(int spawnPosition);

private:
	LevelMainCharacter* m_mainChar;
	// the item can only be picked up if the main char is in this range
	const float PICKUP_RANGE = 100.f;
	std::string m_itemID;
	// only relevant for gold items. they have their item type "Gold".
	int m_goldValue;
	int m_spawnPosition = -1;
	ItemType m_itemType;
	BitmapText m_tooltipText;
	sf::Time m_tooltipTime = sf::Time::Zero;
};