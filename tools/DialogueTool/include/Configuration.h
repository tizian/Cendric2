#pragma once

#include "global.h"

// describes the configuration that can be modified by the user.
// it stores mainly paths.
struct Configuration {
	std::string npcFolder;
	std::string sqlFolder;
	std::string dialogueFolder;
};