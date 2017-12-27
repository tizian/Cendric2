#pragma once

#include "global.h"
#include "Steam/AchievementData.h"
#include "steam-sdk/public/steam/steam_api.h"

class SteamAchievements {
public:
	SteamAchievements(AchievementData* achievements, int achievementCount);
	~SteamAchievements();

	bool requestStats();
	bool setAchievement(const char* ID);

private:
	void onUserStatsReceived(UserStatsReceived_t* pCallback);
	void onUserStatsStored(UserStatsStored_t* pCallback);
	void onAchievementStored(UserAchievementStored_t* pCallback);

private:
	int64 m_appID; // Our current AppID
	AchievementData* m_achievements; // Achievements data
	int m_achievementCount; // The number of Achievements
	bool m_isInitialized; // Have we called Request stats and received the callback?

	CCallback<SteamAchievements, UserStatsStored_t> m_callbackUserStatsStored;
	CCallback<SteamAchievements, UserStatsReceived_t> m_callbackUserStatsReceived;
	CCallback<SteamAchievements, UserAchievementStored_t> m_callbackAchievementStored;
};