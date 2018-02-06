#pragma once

#include "global.h"
#include "World/AnimatedGameObject.h"
#include "ResourceManager.h"
#include "TextProvider.h"
#include "World/Item.h"
#include "World/LightObject.h"

#include "GUI/BitmapText.h"
#include "Enums/ItemType.h"

class LevelMainCharacter;
class LevelScreen;

// A item cendric can collect in a level
class LevelItem final : public AnimatedGameObject {
public:
	LevelItem(LevelScreen* levelScreen);

	void load(const std::string& itemID, const sf::Vector2f& position);
	void onRightClick() override;
	void pickup();

	GameObjectType getConfiguredType() const override;

	void setSpawnPosition(int spawnPosition);
	const std::string& getID() const { return m_itemID; }
	const ItemType getItemType() const { return m_itemType; }

private:
	LevelMainCharacter* m_mainChar;
	
	// the item can only be picked up if the main char is in this range
	const float PICKUP_RANGE = 80.f;
	std::string m_itemID;
	// only relevant for gold items. they have their item type "Gold".
	int m_goldValue;
	int m_spawnPosition = -1;
	ItemType m_itemType;
};