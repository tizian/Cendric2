#pragma once

#include <vector>
#include <map>

#include "global.h"

#include "Structs/ItemBean.h"

// contains information for each and every item and fills item beans.
class ItemFactory
{
public:
	// fills the item bean for the item with id 'id'. 
	// this method assumes the referenced bean is already initialized with default values (DEFAULT_ITEM)
	void loadItemBean(ItemBean& bean, ItemID id);
};