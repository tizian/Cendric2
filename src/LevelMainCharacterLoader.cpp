#include "LevelMainCharacterLoader.h"
#include "LevelMainCharacter.h"

using namespace std;

LevelMainCharacter* LevelMainCharacterLoader::loadMainCharacter(Screen* screen, Level* level) const
{
	LevelMainCharacter* mainChar = new LevelMainCharacter(level);
	mainChar->setCharacterCore(screen->getCharacterCore());
	screen->addObject(GameObjectType::_MainCharacter, mainChar);
	return mainChar;
}

void LevelMainCharacterLoader::loadEquipment(Screen* screen) const
{
	LevelMainCharacter* mainCharacter = dynamic_cast<LevelMainCharacter*>(screen->getObjects(GameObjectType::_MainCharacter)->at(0));
	if (mainCharacter == nullptr)
	{
		g_logger->logError("LevelMainCharacterLoader", "Could not find main character of game screen");
		return;
	}

	// the order of the ids in this vector determine the update and rendering order.
	vector<LevelEquipmentID> gameData;
	gameData.push_back(screen->getCharacterCore()->getEquippedItem(ItemType::Equipment_head).getLevelEquipmentID());
	gameData.push_back(screen->getCharacterCore()->getEquippedItem(ItemType::Equipment_body).getLevelEquipmentID());
	gameData.push_back(screen->getCharacterCore()->getEquippedItem(ItemType::Equipment_back).getLevelEquipmentID());
	gameData.push_back(screen->getCharacterCore()->getEquippedItem(ItemType::Equipment_neck).getLevelEquipmentID());
	gameData.push_back(screen->getCharacterCore()->getEquippedItem(ItemType::Equipment_ring_2).getLevelEquipmentID());
	gameData.push_back(screen->getCharacterCore()->getEquippedItem(ItemType::Equipment_ring_1).getLevelEquipmentID());
	gameData.push_back(screen->getCharacterCore()->getEquippedItem(ItemType::Equipment_weapon).getLevelEquipmentID());

	for (std::vector<LevelEquipmentID>::iterator it = gameData.begin(); it != gameData.end(); ++it)
	{
		if ((*it) == LevelEquipmentID::Void) 
		{
			continue;
		}
		LevelEquipmentBean equipment;
		bool useStandardFrames = false;
		switch (*it)
		{
		case LevelEquipmentID::Weapon_icestaff:
			equipment.textureID = ResourceID::Texture_weapon_icestaff;
			useStandardFrames = true;
			break;
		case LevelEquipmentID::Weapon_rustysword:
			equipment.textureID = ResourceID::Texture_weapon_rustysword;
			useStandardFrames = true;
			break;
		case LevelEquipmentID::Head_wizardhat_blue:
			equipment.textureID = ResourceID::Texture_head_wizardhat_blue;
			useStandardFrames = true;
			break;
		case LevelEquipmentID::Head_wizardhat_grey:
			equipment.textureID = ResourceID::Texture_head_wizardhat_grey;
			useStandardFrames = true;
			break;
		default:
			// unexpected error
			g_logger->logError("LevelMainCharacterLoader", "Equipment item was not loaded, unknown id.");
			return;
		}

		if (useStandardFrames)
		{
			equipment.spriteOffset = sf::Vector2f(0.f, 0.f);
			equipment.boundingBox = sf::FloatRect(0, 0, 120, 120);
			for (int i = 0; i < 8; i++)
			{
				equipment.texturePositions[GameObjectState::Walking].push_back(sf::IntRect(i * 120, 0, 120, 120));
			}
			for (int i = 0; i < 2; i++)
			{
				equipment.texturePositions[GameObjectState::Idle].push_back(sf::IntRect(960 + i * 120, 0, 120, 120));
			}
			for (int i = 0; i < 2; i++)
			{
				equipment.texturePositions[GameObjectState::Jumping].push_back(sf::IntRect(1200 + i * 120, 0, 120, 120));
			}
			for (int i = 0; i < 5; i++)
			{
				equipment.texturePositions[GameObjectState::Fighting].push_back(sf::IntRect(1440 + i * 120, 0, 120, 120));
			}
			equipment.frameTime = sf::seconds(0.07f);
		}

		LevelEquipment* levelEquipment = new LevelEquipment();
		levelEquipment->setBoundingBox(equipment.boundingBox);
		for (auto &ani : equipment.texturePositions)
		{
			Animation animation;
			animation.setSpriteSheet(g_resourceManager->getTexture(equipment.textureID));
			for (auto &frame : ani.second) 
			{
				animation.addFrame(frame);
			}
			levelEquipment->addAnimation(ani.first, animation);
		}

		levelEquipment->setFrameTime(equipment.frameTime);
		// initial values
		levelEquipment->setCurrentAnimation(levelEquipment->getAnimation(GameObjectState::Idle), false);
		levelEquipment->playCurrentAnimation(true);
		levelEquipment->loadEquipment(mainCharacter, (*it));
		levelEquipment->setTextureID(equipment.textureID);
		screen->addObject(GameObjectType::_LevelEquipment, levelEquipment);
	}
}