#pragma once

#include "global.h"
#include "Enums/ItemID.h"
#include "Enums/ItemType.h"
#include "Enums/ResourceID.h"
#include "Structs/ItemBean.h"

// An item in cendrics / a npcs / a mobs inventory
class Item 
{
public:
	Item(ItemBean& bean);

	const std::string& getDescription() const;
	ItemID getID() const;
	ItemType getType() const;
	ResourceID getIcon() const;
	// the items gold value
	int getValue() const;

private:
	ItemBean m_bean;
};