#pragma once

#include "global.h"
#include "FileIO/CSVReader.h"

// reads items from a specified csv file
// used by the resource manager.
class ItemReader : public CSVReader
{
public:
	ItemReader();
	~ItemReader();

	// clears the item map and fills it again with the items from the csv file
	// returns if the operation was successful
	bool readItems(Language lang, std::map<std::string, std::wstring>& translationMap) const;
};