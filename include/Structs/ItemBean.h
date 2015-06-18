#pragma once

#include <iostream>

#include "Enums/ItemID.h"
#include "Enums/ItemType.h"
#include "Enums/LevelEquipmentID.h"

#include "Structs/AttributeBean.h"

struct ItemBean
{
	std::string description;
	std::string name;
	ItemID id;
	ItemType type;
	// in texture coordinates. The location is the spritesheet for items and the height and width is always 50px.
	sf::Vector2i iconTextureLocation;
	// only relevant for equipment, else void.
	LevelEquipmentID levelEquipment;
	AttributeBean attributes;
	int value;
};

const struct ItemBean DEFAULT_ITEM = 
{
	"",
	"",
	ItemID::Void,
	ItemType::Void,
	sf::Vector2i(),
	LevelEquipmentID::Void,
	ZERO_ATTRIBUTES,
	0
};