#include "Steam/AchievementManager.h"

#ifdef STEAM
	#include "steam-sdk/public/steam/steam_api.h"
#endif // STEAM


AchievementManager::AchievementManager() {
#ifdef STEAM
	const bool isSuccess = SteamAPI_Init();

	if (isSuccess)
	{
		m_achievements = new SteamAchievements(g_Achievements, 4);
	}
#endif // STEAM
}

AchievementManager::~AchievementManager() {
#ifdef STEAM
	SteamAPI_Shutdown();
#endif // STEAM
}

void AchievementManager::unlockAchievement(const std::string& achievement) {
	// TODO
}