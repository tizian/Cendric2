#include "Steam/AchievementManager.h"

#ifdef STEAM
#include "Steam/CendricAchievements.h"
#include "Steam/SteamAchievements.h"
#include "Steam/Achievement.h"
#endif

AchievementManager::AchievementManager(CharacterCore* core) {
	m_characterCore = core;
#ifdef STEAM
	const bool isSuccess = SteamAPI_Init();

	if (isSuccess)
	{
		m_steamAchievements = new SteamAchievements(CendricAchievements, ACHIEVEMENT_COUNT);
	}

	initAchievements();
#endif // STEAM
}

AchievementManager::~AchievementManager() {
#ifdef STEAM
	SteamAPI_Shutdown();
	delete m_steamAchievements;

	for (auto kv : m_achievements) {
		delete kv.second;
	}
	m_achievements.clear();
#endif // STEAM
}

void AchievementManager::initAchievements() {
#ifdef STEAM

#endif
}

void AchievementManager::notifyAchievement(const std::string& achievement, const std::string& tag, const std::string& message) {
#ifdef STEAM
	if (!contains(m_achievements, achievement)) return;

	if (m_achievements.at(achievement)->notify(tag, message)) {
		unlockAchievement(achievement);
	}
#endif // STEAM
}

void AchievementManager::notifyAchievementCore(const std::string& achievement) {
#ifdef STEAM
	if (!contains(m_achievements, achievement)) return;

	if (m_achievements.at(achievement)->notifyCore(m_characterCore)) {
		unlockAchievement(achievement);
	}
#endif // STEAM
}

void AchievementManager::unlockAchievement(const std::string& achievement) {
#ifdef STEAM
	if (!m_steamAchievements) return;
	if (!contains(m_achievements, achievement)) return;

	auto it = m_achievements.find(achievement);
	delete (*it).second;
	m_achievements.erase(it);
	m_steamAchievements->setAchievement(achievement.c_str());
#endif // STEAM
}
