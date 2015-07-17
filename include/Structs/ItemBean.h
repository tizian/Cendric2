#pragma once

#include <iostream>

#include "Enums/ItemType.h"
#include "Structs/AttributeBean.h"

// an item bean, directly read from a csv file
// it contains all information for items, their level items and (maybe) equipment
struct ItemBean
{
	// for item
	std::string id;
	std::string description;
	ItemType type;
	// in texture coordinates. The location is the spritesheet for items and the height and width is always 50px.
	sf::Vector2i iconTextureLocation;
	int goldValue;
	AttributeBean attributes;
	sf::Time foodDuration;
	
	// For levelItem
	sf::Vector2f spriteOffset;
	sf::FloatRect boundingBox;
	std::vector<sf::IntRect> texturePositions;
	sf::Time frameTime;

	// for equipment items
	std::string spritesheetPath;
};

const struct ItemBean DEFAULT_ITEM = 
{
	"",
	"",
	ItemType::VOID,
	sf::Vector2i(),
	0,
	ZERO_ATTRIBUTES,
	sf::Time::Zero,

	sf::Vector2f(),
	sf::FloatRect(),
	std::vector<sf::IntRect>(),
	sf::seconds(1),

	""
};