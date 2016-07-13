#include "Map/MapMainCharacterLoader.h"
#include "Map/MapMainCharacter.h"
#include "Screens/MapScreen.h"
#include "Animation.h"

const int EQ_SIZE = 120;

MapMainCharacter* MapMainCharacterLoader::loadMainCharacter(Screen* screen, Map* map) const {
	MapMainCharacter* mainChar = new MapMainCharacter(map);
	screen->addObject(mainChar);
	mainChar->load();
	mainChar->setCharacterCore(screen->getCharacterCore());
	return mainChar;
}

void MapMainCharacterLoader::loadEquipment(Screen* screen) const {
	MapMainCharacter* mainCharacter = dynamic_cast<MapScreen*>(screen)->getMainCharacter();
	if (mainCharacter == nullptr) {
		g_logger->logError("MapMainCharacterLoader", "Could not find main character of game screen");
		return;
	}

	// the order of the ids in this vector determine the update and rendering order. 
	std::vector<ItemType> equipmentOrder;

	equipmentOrder.push_back(ItemType::Equipment_body);
	equipmentOrder.push_back(ItemType::Equipment_head);
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
		if (item_ == nullptr || !item_->isValid()) {
			g_logger->logError("LevelMainCharacterLoader", "Equipment item was not loaded, unknown id: " + it);
			continue;
		}
		Item& item = *item_;
		if (!item.isEquipmentItem() || item.getEquipmentBean().map_texture_path.empty()) {
			// this item has no equipment part
			continue;
		}

		const std::string& texturePath = item.getEquipmentBean().map_texture_path;
		g_resourceManager->loadTexture(texturePath, ResourceType::Map);
		const sf::Texture* tex = g_resourceManager->getTexture(texturePath);

		MapEquipment* mapEquipment = new MapEquipment(mainCharacter);

		mapEquipment->setBoundingBox(*mainCharacter->getBoundingBox());
		mapEquipment->setSpriteOffset(mainCharacter->getSpriteOffset())

		Animation* walkingAnimationDown = new Animation(sf::seconds(0.15f));
		walkingAnimationDown->setSpriteSheet(tex);
		walkingAnimationDown->addFrame(sf::IntRect(0, 0, 50, 50));
		walkingAnimationDown->addFrame(sf::IntRect(50, 0, 50, 50));
		walkingAnimationDown->addFrame(sf::IntRect(100, 0, 50, 50));
		walkingAnimationDown->addFrame(sf::IntRect(50, 0, 50, 50));

		addAnimation(GameObjectState::Walking_down, walkingAnimationDown);

		Animation* walkingAnimationLeft = new Animation(sf::seconds(0.15f));
		walkingAnimationLeft->setSpriteSheet(tex);
		walkingAnimationLeft->addFrame(sf::IntRect(0, 50, 50, 50));
		walkingAnimationLeft->addFrame(sf::IntRect(50, 50, 50, 50));
		walkingAnimationLeft->addFrame(sf::IntRect(100, 50, 50, 50));
		walkingAnimationLeft->addFrame(sf::IntRect(50, 50, 50, 50));

		addAnimation(GameObjectState::Walking_left, walkingAnimationLeft);

		Animation* walkingAnimationRight = new Animation(sf::seconds(0.15f));
		walkingAnimationRight->setSpriteSheet(tex);
		walkingAnimationRight->addFrame(sf::IntRect(0, 100, 50, 50));
		walkingAnimationRight->addFrame(sf::IntRect(50, 100, 50, 50));
		walkingAnimationRight->addFrame(sf::IntRect(100, 100, 50, 50));
		walkingAnimationRight->addFrame(sf::IntRect(50, 100, 50, 50));

		addAnimation(GameObjectState::Walking_right, walkingAnimationRight);

		Animation* walkingAnimationUp = new Animation(sf::seconds(0.15f));
		walkingAnimationUp->setSpriteSheet(tex);
		walkingAnimationUp->addFrame(sf::IntRect(0, 150, 50, 50));
		walkingAnimationUp->addFrame(sf::IntRect(50, 150, 50, 50));
		walkingAnimationUp->addFrame(sf::IntRect(100, 150, 50, 50));
		walkingAnimationUp->addFrame(sf::IntRect(50, 150, 50, 50));

		addAnimation(GameObjectState::Walking_up, walkingAnimationUp);

		Animation* idleAnimationDown = new Animation();
		idleAnimationDown->setSpriteSheet(tex);
		idleAnimationDown->addFrame(sf::IntRect(50, 0, 50, 50));

		addAnimation(GameObjectState::Idle_down, idleAnimationDown);

		Animation* idleAnimationLeft = new Animation();
		idleAnimationLeft->setSpriteSheet(tex);
		idleAnimationLeft->addFrame(sf::IntRect(50, 50, 50, 50));

		addAnimation(GameObjectState::Idle_left, idleAnimationLeft);

		Animation* idleAnimationRight = new Animation();
		idleAnimationRight->setSpriteSheet(tex);
		idleAnimationRight->addFrame(sf::IntRect(50, 100, 50, 50));

		addAnimation(GameObjectState::Idle_right, idleAnimationRight);

		Animation* idleAnimationUp = new Animation();
		idleAnimationUp->setSpriteSheet(tex);
		idleAnimationUp->addFrame(sf::IntRect(50, 150, 50, 50));

		addAnimation(GameObjectState::Idle_up, idleAnimationUp);

		setDebugBoundingBox(COLOR_WHITE);

		// initial values
		m_state = GameObjectState::Idle_down;
		setCurrentAnimation(getAnimation(m_state), false);
		playCurrentAnimation(true);

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

		screen->addObject(mapEquipment);
	}
}