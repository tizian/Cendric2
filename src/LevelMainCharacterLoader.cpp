#include "LevelMainCharacterLoader.h"
#include "LevelMainCharacter.h"

using namespace std;

LevelMainCharacter* LevelMainCharacterLoader::loadMainCharacter(Screen* screen, Level* level) const {
	LevelMainCharacter* mainChar = new LevelMainCharacter(level);
	screen->addObject(mainChar);
	mainChar->load();
	mainChar->setCharacterCore(screen->getCharacterCore());
	return mainChar;
}

void LevelMainCharacterLoader::loadEquipment(Screen* screen) const {
	LevelMainCharacter* mainCharacter = dynamic_cast<LevelMainCharacter*>(screen->getObjects(GameObjectType::_MainCharacter)->at(0));
	if (mainCharacter == nullptr) {
		g_logger->logError("LevelMainCharacterLoader", "Could not find main character of game screen");
		return;
	}

	// the order of the ids in this vector determine the update and rendering order. 
	vector<ItemType> equipmentOrder;
	equipmentOrder.push_back(ItemType::Equipment_weapon);
	equipmentOrder.push_back(ItemType::Equipment_ring_1);
	equipmentOrder.push_back(ItemType::Equipment_ring_2);
	equipmentOrder.push_back(ItemType::Equipment_neck);
	equipmentOrder.push_back(ItemType::Equipment_back);
	equipmentOrder.push_back(ItemType::Equipment_body);
	equipmentOrder.push_back(ItemType::Equipment_head);
	vector<string> gameData;
	for (auto& it : equipmentOrder) {
		if (screen->getCharacterCore()->getEquippedItem(it) == nullptr) continue;
		gameData.push_back(screen->getCharacterCore()->getEquippedItem(it)->getID());
	}

	for (auto& it : gameData) {
		LevelEquipmentBean equipment;
		const ItemBean* bean = g_resourceManager->getItemBean(it);
		if (bean == nullptr) {
			g_logger->logError("LevelMainCharacterLoader", "Equipment item was not loaded, unknown id.");
			return;
		}

		equipment.texturePath = bean->spritesheetPath;

		if (equipment.texturePath.empty()) continue;

		equipment.spriteOffset = sf::Vector2f(0.f, 0.f);
		equipment.boundingBox = sf::FloatRect(0, 0, 120, 120);
		for (int i = 0; i < 8; i++) {
			equipment.texturePositions[GameObjectState::Walking].push_back(sf::IntRect(i * 120, 0, 120, 120));
		}
		for (int i = 0; i < 2; i++) {
			equipment.texturePositions[GameObjectState::Idle].push_back(sf::IntRect(960 + i * 120, 0, 120, 120));
		}
		for (int i = 0; i < 2; i++) {
			equipment.texturePositions[GameObjectState::Jumping].push_back(sf::IntRect(1200 + i * 120, 0, 120, 120));
		}
		for (int i = 0; i < 5; i++) {
			equipment.texturePositions[GameObjectState::Fighting].push_back(sf::IntRect(1440 + i * 120, 0, 120, 120));
		}
		equipment.frameTime = sf::seconds(0.07f);

		LevelEquipment* levelEquipment = new LevelEquipment();
		levelEquipment->setBoundingBox(equipment.boundingBox);
		for (auto &ani : equipment.texturePositions) {
			Animation animation;
			animation.setSpriteSheet(g_resourceManager->getTexture(bean->spritesheetPath));
			for (auto &frame : ani.second) {
				animation.addFrame(frame);
			}
			levelEquipment->addAnimation(ani.first, animation);
		}

		levelEquipment->setFrameTime(equipment.frameTime);
		// initial values
		levelEquipment->setCurrentAnimation(levelEquipment->getAnimation(GameObjectState::Idle), false);
		levelEquipment->playCurrentAnimation(true);
		levelEquipment->loadEquipment(mainCharacter);
		screen->addObject(levelEquipment);
	}
}