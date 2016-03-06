#pragma once

#include "global.h"
#include "AnimatedGameObject.h"
#include "ResourceManager.h"
#include "TextProvider.h"
#include "Item.h"
#include "LightObject.h"

#include "GUI/BitmapText.h"
#include "Enums/ItemType.h"

class LevelMainCharacter;

// A item cendric can collect in a level
class LevelItem : public AnimatedGameObject {
public:
	LevelItem() : AnimatedGameObject() {}
	void load(LevelMainCharacter* mainChar, const Item& item, const sf::Vector2f& position);
	void onRightClick() override;
	void onMouseOver() override;
	void pickup();
	void render(sf::RenderTarget& renderTarget) override;
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;
	void update(const sf::Time& frameTime) override;

	void setLightObject(LightObject* lightObject);
	void setScreen(Screen* screen) override;
	void setPosition(const sf::Vector2f& position) override;
	void setDisposed() override;

	GameObjectType getConfiguredType() const override;

	void setTooltipText(const std::string& tooltip);
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

	LightObject* m_lightObject = nullptr;
	sf::Vector2f m_lightObjectOffset;
};