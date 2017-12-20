#pragma once

#include "global.h"

class AchievementManager final {
public:
	AchievementManager();
	~AchievementManager();

	void unlockAchievement(const std::string& achievement);

private:
};