#pragma once

#include "global.h"
#include "Steam/SteamAchievements.h"

class AchievementManager final {
public:
	AchievementManager();
	~AchievementManager();

	void unlockAchievement(const std::string& achievement);

private:
	SteamAchievements* m_achievements = nullptr;
};