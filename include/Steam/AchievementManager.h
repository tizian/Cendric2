#pragma once

#include "global.h"

class SteamAchievements;

class AchievementManager final {
public:
	AchievementManager();
	~AchievementManager();

	void unlockAchievement(const std::string& achievement);

private:
	SteamAchievements* m_achievements = nullptr;
};