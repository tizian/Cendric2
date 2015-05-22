#pragma once

#include <iostream>

#include "Enums/ItemID.h"
#include "Enums/ItemType.h"
#include "Enums/ResourceID.h"
#include "Enums/Texts.h"
#include "Enums/LevelEquipmentID.h"

#include "Structs/AttributeBean.h"

struct ItemBean
{
	Texts description;
	ItemID id;
	ItemType type;
	ResourceID icon;
	// only relevant for equipment, else void.
	LevelEquipmentID levelEquipment;
	AttributeBean attributes;
	int value;
};

const struct ItemBean DEFAULT_ITEM = 
{
	Texts::Void, 
	ItemID::Void, 
	ItemType::Void, 
	ResourceID::Void,
	LevelEquipmentID::Void,
	ZERO_ATTRIBUTES,
	0
};