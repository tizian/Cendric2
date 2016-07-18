#include "Level/LevelMainCharacterLoader.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"
#include "GameObjectComponents/LightComponent.h"

using namespace std;

const int EQ_SIZE = 120;

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
		LevelEquipmentData equipment;
		Item* item_ = g_resourceManager->getItem(it);
		if (item_ == nullptr || !item_->isValid()) {
			g_logger->logError("LevelMainCharacterLoader", "Equipment item was not loaded, unknown id: " + it);
			continue;
		}
		Item& item = *item_;
		if (!item.isEquipmentItem()) {
			// this item has no equipment part
			continue;
		}

		const ItemEquipmentBean& eq = item.getEquipmentBean();
		equipment.texturePath = eq.texture_path;

		equipment.spriteOffset = sf::Vector2f(0.f, 0.f);
		equipment.boundingBox = sf::FloatRect(0, 0, static_cast<float>(EQ_SIZE), static_cast<float>(EQ_SIZE));
		int offset = 0;
		for (int i = 0; i < eq.frames_walk; ++i) {
			equipment.texturePositions[GameObjectState::Walking].push_back(sf::IntRect(offset * EQ_SIZE, 0, EQ_SIZE, EQ_SIZE));
			++offset;
		}
		for (int i = 0; i < eq.frames_idle; ++i) {
			equipment.texturePositions[GameObjectState::Idle].push_back(sf::IntRect(offset * EQ_SIZE, 0, EQ_SIZE, EQ_SIZE));
			++offset;
		}
		for (int i = 0; i < eq.frames_jump; ++i) {
			equipment.texturePositions[GameObjectState::Jumping].push_back(sf::IntRect(offset * EQ_SIZE, 0, EQ_SIZE, EQ_SIZE));
			++offset;
		}
		for (int i = 0; i < eq.frames_fight; ++i) {
			equipment.texturePositions[GameObjectState::Fighting].push_back(sf::IntRect(offset * EQ_SIZE, 0, EQ_SIZE, EQ_SIZE));
			++offset;
		}
		for (int i = 0; i < eq.frames_climb1; ++i) {
			equipment.texturePositions[GameObjectState::Climbing_1].push_back(sf::IntRect(offset * EQ_SIZE, 0, EQ_SIZE, EQ_SIZE));
			++offset;
		}
		for (int i = 0; i < eq.frames_climb2; ++i) {
			equipment.texturePositions[GameObjectState::Climbing_2].push_back(sf::IntRect(offset * EQ_SIZE, 0, EQ_SIZE, EQ_SIZE));
			++offset;
		}

		LevelEquipment* levelEquipment = new LevelEquipment(mainCharacter);
		levelEquipment->setBoundingBox(equipment.boundingBox);

		if (item.isEquipmentLightedItem()) {
			const ItemEquipmentLightBean& lightBean = item.getEquipmentLightBean();
			LightData lightData(LightData(lightBean.light_offset, lightBean.light_radius, lightBean.brightness));
			levelEquipment->setLightComponent(lightData);
		}

		if (!equipment.texturePath.empty()) {
			g_resourceManager->loadTexture(equipment.texturePath, ResourceType::Level);
			for (auto& ani : equipment.texturePositions) {
				Animation* animation = new Animation();
				if (ani.first == GameObjectState::Fighting) {
					animation->setFrameTime(sf::milliseconds(70));
				}
				animation->setSpriteSheet(g_resourceManager->getTexture(item.getEquipmentBean().texture_path));
				for (auto& frame : ani.second) {
					animation->addFrame(frame);
				}
				levelEquipment->addAnimation(ani.first, animation);
			}

			// initial values
			levelEquipment->setCurrentAnimation(levelEquipment->getAnimation(GameObjectState::Idle), false);
			levelEquipment->playCurrentAnimation(true);

			levelEquipment->setHasTexture();
		}

		sf::Vector2f newPosition;
		levelEquipment->calculatePositionAccordingToMainChar(newPosition);
		levelEquipment->setPosition(newPosition);
		screen->addObject(levelEquipment);
	}
}