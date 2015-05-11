#pragma once

#include <vector>
#include <map>

#include "global.h"
#include "Screen.h"
#include "LevelEquipment.h"
#include "MainCharacter.h"
#include "TextProvider.h"
#include "Level.h"

struct LevelEquipmentBean
{
	sf::Vector2f spriteOffset;
	sf::FloatRect boundingBox;
	ResourceID textureID;
	std::map<GameObjectState, std::vector<sf::IntRect>> texturePositions;
	sf::Time frameTime;
};

// helper class used by the game screan to load the main character including his weapon / equipment.
class MainCharacterLoader
{
public:
	// loads the main character and adds it directly to the screen
	MainCharacter* loadMainCharacter(Screen* screen, Level* level) const;
	// loads level items out of level data and adds them directly to the screen
	void loadEquipment(Screen* screen) const;
};