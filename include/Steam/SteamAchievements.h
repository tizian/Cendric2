class SteamAchievements {
public:
	SteamAchievements(AchievementData* achievements, int achievementCount);
	~SteamAchievements();

	bool requestStats();
	bool setAchievement(const char* ID);

	STEAM_CALLBACK(SteamAchievements, onUserStatsReceived, UserStatsReceived_t,
		m_callbackUserStatsReceived );
	STEAM_CALLBACK(SteamAchievements, onUserStatsStored, UserStatsStored_t,
		m_callbackUserStatsStored );
	STEAM_CALLBACK(SteamAchievements, onAchievementStored,
		UserAchievementStored_t, m_callbackAchievementStored );
		
private:
	int64 m_appID; // Our current AppID
	AchievementData* m_achievements; // Achievements data
	int m_achievementCount; // The number of Achievements
	bool m_isInitialized; // Have we called Request stats and received the callback?
};