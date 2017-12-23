#include "Steam/AchievementManager.h"

#ifdef STEAM
#include "Steam/CendricAchievements.h"
#include "Steam/SteamAchievements.h"
#endif

AchievementManager::AchievementManager() {
#ifdef STEAM
	const bool isSuccess = SteamAPI_Init();

	if (isSuccess)
	{
		m_achievements = new SteamAchievements(CendricAchievements, 28);
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