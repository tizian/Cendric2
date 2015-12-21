#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <set>

struct QuestData {
	std::string id;
	std::string title;

	// collect quest: which items must be collected and how many of them?
	// first element is the item id, the second the amount
	std::map<std::string, int> collectibles;
	// kill quest: which enemies must be killed and how many of them?
	// first element is the enemy name, the second the amount
	std::map<std::string, int> targets;
	// which conditions must be fulfilled?
	std::set<std::string> conditions;
};