#pragma once

#include "global.h"
#include "FileIO/CSVReader.h"

#include "Structs/ItemBean.h"

// reads items from a specified csv file
// used by the resource manager.
class ItemReader : public CSVReader {
public:
	ItemReader();
	~ItemReader();

	// clears the item map and fills it again with the items from the csv file
	// returns if the operation was successful
	bool readItems(std::map<std::string, ItemBean>& itemMap) const;

private:
	// checks an item bean for validity
	bool checkItem(const ItemBean& bean) const;
};