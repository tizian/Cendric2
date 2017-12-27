#pragma once

#include "global.h"
#include "Steam/CendricAchievements.h"

class CharacterCore;

class Achievement {
public:
	Achievement() {}
	virtual ~Achievement() {};
    
	virtual bool notify(const std::string& tag, const std::string& message) { return false; };
	virtual bool notifyCore(const CharacterCore* core) { return false; }
};