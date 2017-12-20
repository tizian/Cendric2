#include "AchievementManager.h"

#ifdef STEAM
	#include "steam-sdk/public/steam/steam_api.h"
#endif // STEAM


AchievementManager::AchievementManager() {
#ifdef STEAM
	SteamAPI_Init();
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