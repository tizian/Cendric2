#include "Map/MapMainCharacterLoader.h"
#include "Map/MapMainCharacter.h"
#include "Screens/MapScreen.h"
#include "World/Item.h"

MapMainCharacter* MapMainCharacterLoader::loadMainCharacter(Screen* screen, Map* map) {
	MapMainCharacter* mainChar = new MapMainCharacter(map, screen);
	screen->addObject(mainChar);
	mainChar->load();
	mainChar->setCharacterCore(screen->getCharacterCore());
	return mainChar;
}

void MapMainCharacterLoader::loadEquipment(Screen* screen) {
	MapMainCharacter* mainCharacter = dynamic_cast<MapScreen*>(screen)->getMainCharacter();
	if (mainCharacter == nullptr) {
		g_logger->logError("MapMainCharacterLoader", "Could not find main character of game screen");
		return;
	}

	// make sure to clear all equipment pieces.
	for (auto& go : *screen->getObjects(GameObjectType::_Equipment)) {
		go->setDisposed();
	}

	// make sure that the main character is in the correct animation state (first frame)
	mainCharacter->setCurrentAnimation(mainCharacter->getAnimation(mainCharacter->getGameObjectState()), false);

	// the order of the ids in this vector determine the update and rendering order. 
	std::vector<ItemType> equipmentOrder;

	equipmentOrder.push_back(ItemType::Equipment_head);
	equipmentOrder.push_back(ItemType::Equipment_body);
	equipmentOrder.push_back(ItemType::Equipment_back);
	equipmentOrder.push_back(ItemType::Equipment_weapon);
	equipmentOrder.push_back(ItemType::Equipment_neck);
	equipmentOrder.push_back(ItemType::Equipment_ring_1);
	equipmentOrder.push_back(ItemType::Equipment_ring_2);
	
	std::vector<std::string> gameData;
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

		MapEquipment* mapEquipment = new MapEquipment(mainCharacter);

		mapEquipment->setBoundingBox(*mainCharacter->getBoundingBox());
		mapEquipment->setSpriteOffset(mainCharacter->getSpriteOffset());
		mapEquipment->setDebugBoundingBox(COLOR_WHITE);

		if (item.getCheck().isEquipmentLighted) {
			const ItemEquipmentLightBean& lightBean = *item.getBean<ItemEquipmentLightBean>();
			if (lightBean.map_light_radius.x > 0.f && lightBean.map_light_radius.y > 0.f) {
				LightData lightData(LightData(sf::Vector2f(9.f, -30.f), lightBean.map_light_radius, lightBean.brightness));
				mapEquipment->setLightComponent(lightData);
			}
		}

		auto const equipmentBean = item.getBean<ItemEquipmentBean>();
		bool hasTexture = !equipmentBean->map_texture_path.empty();
		if (hasTexture) {
			const std::string& texturePath = equipmentBean->map_texture_path;
			g_resourceManager->loadTexture(texturePath, ResourceType::Map);
			const sf::Texture* tex = g_resourceManager->getTexture(texturePath);

			Animation* walkingAnimationDown = new Animation(sf::seconds(0.15f));
			walkingAnimationDown->setSpriteSheet(tex);
			walkingAnimationDown->addFrame(sf::IntRect(0, 0, 50, 50));
			walkingAnimationDown->addFrame(sf::IntRect(50, 0, 50, 50));
			walkingAnimationDown->addFrame(sf::IntRect(100, 0, 50, 50));
			walkingAnimationDown->addFrame(sf::IntRect(50, 0, 50, 50));

			mapEquipment->addAnimation(GameObjectState::Walking_down, walkingAnimationDown);

			Animation* walkingAnimationLeft = new Animation(sf::seconds(0.15f));
			walkingAnimationLeft->setSpriteSheet(tex);
			walkingAnimationLeft->addFrame(sf::IntRect(0, 50, 50, 50));
			walkingAnimationLeft->addFrame(sf::IntRect(50, 50, 50, 50));
			walkingAnimationLeft->addFrame(sf::IntRect(100, 50, 50, 50));
			walkingAnimationLeft->addFrame(sf::IntRect(50, 50, 50, 50));

			mapEquipment->addAnimation(GameObjectState::Walking_left, walkingAnimationLeft);

			Animation* walkingAnimationRight = new Animation(sf::seconds(0.15f));
			walkingAnimationRight->setSpriteSheet(tex);
			walkingAnimationRight->addFrame(sf::IntRect(0, 100, 50, 50));
			walkingAnimationRight->addFrame(sf::IntRect(50, 100, 50, 50));
			walkingAnimationRight->addFrame(sf::IntRect(100, 100, 50, 50));
			walkingAnimationRight->addFrame(sf::IntRect(50, 100, 50, 50));

			mapEquipment->addAnimation(GameObjectState::Walking_right, walkingAnimationRight);

			Animation* walkingAnimationUp = new Animation(sf::seconds(0.15f));
			walkingAnimationUp->setSpriteSheet(tex);
			walkingAnimationUp->addFrame(sf::IntRect(0, 150, 50, 50));
			walkingAnimationUp->addFrame(sf::IntRect(50, 150, 50, 50));
			walkingAnimationUp->addFrame(sf::IntRect(100, 150, 50, 50));
			walkingAnimationUp->addFrame(sf::IntRect(50, 150, 50, 50));

			mapEquipment->addAnimation(GameObjectState::Walking_up, walkingAnimationUp);

			Animation* idleAnimationDown = new Animation();
			idleAnimationDown->setSpriteSheet(tex);
			idleAnimationDown->addFrame(sf::IntRect(50, 0, 50, 50));

			mapEquipment->addAnimation(GameObjectState::Idle_down, idleAnimationDown);

			Animation* idleAnimationLeft = new Animation();
			idleAnimationLeft->setSpriteSheet(tex);
			idleAnimationLeft->addFrame(sf::IntRect(50, 50, 50, 50));

			mapEquipment->addAnimation(GameObjectState::Idle_left, idleAnimationLeft);

			Animation* idleAnimationRight = new Animation();
			idleAnimationRight->setSpriteSheet(tex);
			idleAnimationRight->addFrame(sf::IntRect(50, 100, 50, 50));

			mapEquipment->addAnimation(GameObjectState::Idle_right, idleAnimationRight);

			Animation* idleAnimationUp = new Animation();
			idleAnimationUp->setSpriteSheet(tex);
			idleAnimationUp->addFrame(sf::IntRect(50, 150, 50, 50));

			mapEquipment->addAnimation(GameObjectState::Idle_up, idleAnimationUp);

			mapEquipment->setHasTexture();
		}

		// initial values
		GameObjectState newState = mainCharacter->getGameObjectState();
		mapEquipment->playCurrentAnimation(true);
		mapEquipment->setState(newState, hasTexture);
		mapEquipment->setPosition(mainCharacter->getPosition());

		screen->addObject(mapEquipment);
	}
}