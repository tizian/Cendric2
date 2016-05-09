#include "Level/LevelMainCharacterLoader.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"
#include "GameObjectComponents/LightComponent.h"

using namespace std;

LevelMainCharacter* LevelMainCharacterLoader::loadMainCharacter(Screen* screen, Level* level) const {
	LevelMainCharacter* mainChar = new LevelMainCharacter(level);
	screen->addObject(mainChar);
	mainChar->load();
	mainChar->setCharacterCore(screen->getCharacterCore());
	return mainChar;
}

void LevelMainCharacterLoader::loadEquipment(Screen* screen) const {
	LevelMainCharacter* mainCharacter = dynamic_cast<LevelScreen*>(screen)->getMainCharacter();
	if (mainCharacter == nullptr) {
		g_logger->logError("LevelMainCharacterLoader", "Could not find main character of game screen");
		return;
	}

	// the order of the ids in this vector determine the update and rendering order. 
	vector<ItemType> equipmentOrder;

	equipmentOrder.push_back(ItemType::Equipment_body);
	equipmentOrder.push_back(ItemType::Equipment_back);
	equipmentOrder.push_back(ItemType::Equipment_head);
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
		LevelEquipmentData equipment;
		Item item = Item(it);
		if (!item.isValid() || !item.isEquipmentItem()) {
			g_logger->logError("LevelMainCharacterLoader", "Equipment item was not loaded, unknown id.");
			return;
		}

		equipment.texturePath = item.getEquipmentBean().texture_path;

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

		LevelEquipment* levelEquipment = new LevelEquipment(mainCharacter);
		levelEquipment->setBoundingBox(equipment.boundingBox);

		if (item.isEquipmentLightedItem()) {
			const ItemEquipmentLightBean& lightBean = item.getEquipmentLightBean();
			LightData lightData(LightData(lightBean.light_offset, lightBean.light_radius, lightBean.brightness));
			levelEquipment->setLightComponent(lightData);
		}

		if (!equipment.texturePath.empty()) {
			for (auto &ani : equipment.texturePositions) {
				Animation* animation = new Animation();
				if (ani.first == GameObjectState::Fighting) {
					animation->setFrameTime(sf::milliseconds(70));
				}
				else if (ani.first == GameObjectState::Jumping) {
					animation->setFrameTime(sf::milliseconds(200));
				}
				animation->setSpriteSheet(g_resourceManager->getTexture(item.getEquipmentBean().texture_path));
				for (auto &frame : ani.second) {
					animation->addFrame(frame);
				}
				levelEquipment->addAnimation(ani.first, animation);
			}

			// initial values
			levelEquipment->setCurrentAnimation(levelEquipment->getAnimation(GameObjectState::Idle), false);
			levelEquipment->playCurrentAnimation(true);
		}

		levelEquipment->setTexturePath(equipment.texturePath);
		levelEquipment->loadEquipment();
		screen->addObject(levelEquipment);
	}
}