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