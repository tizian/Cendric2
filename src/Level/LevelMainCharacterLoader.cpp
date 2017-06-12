#include "Level/LevelMainCharacterLoader.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"
#include "World/Item.h"
#include "GameObjectComponents/LightComponent.h"

using namespace std;

LevelMainCharacter* LevelMainCharacterLoader::loadMainCharacter(Screen* screen, Level* level) {
	LevelMainCharacter* mainChar = new LevelMainCharacter(level);
	screen->addObject(mainChar);
	mainChar->load();
	mainChar->setCharacterCore(screen->getCharacterCore());
	return mainChar;
}

void LevelMainCharacterLoader::loadEquipment(Screen* screen) {
	LevelMainCharacter* mainCharacter = dynamic_cast<LevelScreen*>(screen)->getMainCharacter();
	if (mainCharacter == nullptr) {
		g_logger->logError("LevelMainCharacterLoader", "Could not find main character of game screen");
		return;
	}

	// the order of the ids in this vector determine the update and rendering order. 
	vector<ItemType> equipmentOrder;

	equipmentOrder.push_back(ItemType::Equipment_body);
	equipmentOrder.push_back(ItemType::Equipment_head);
	equipmentOrder.push_back(ItemType::Equipment_back);
	equipmentOrder.push_back(ItemType::Equipment_weapon);
	equipmentOrder.push_back(ItemType::Equipment_neck);
	equipmentOrder.push_back(ItemType::Equipment_ring_1);
	equipmentOrder.push_back(ItemType::Equipment_ring_2);
	
	vector<string> gameData;
	for (auto& it : equipmentOrder) {
		if (screen->getCharacterCore()->getEquippedItem(it).empty()) continue;
		gameData.push_back(screen->getCharacterCore()->getEquippedItem(it));
	}

	for (auto& it : gameData) {
		Item* item_ = g_resourceManager->getItem(it);
		if (item_ == nullptr || !item_->getCheck().isValid) {
			g_logger->logError("LevelMainCharacterLoader", "Equipment item was not loaded, unknown id: " + it);
			continue;
		}
		Item& item = *item_;
		if (!item.getCheck().isEquipment) {
			// this item has no equipment part
			continue;
		}

		LevelEquipment* levelEquipment = new LevelEquipment(mainCharacter);
		levelEquipment->load(item.getBean<ItemEquipmentBean>(), item.getBean<ItemEquipmentLightBean>(), item.getType());
		screen->addObject(levelEquipment);
	}
}