#include "Steam/AchievementManager.h"
#include "CharacterCore.h"
#include "ObjectFactory.h"

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
		m_cendricAchievements = createAchievementsArray();
		m_steamAchievements = new SteamAchievements(m_cendricAchievements, static_cast<int>(AchievementID::MAX) - 1);
	}

	initAchievements();
#endif // STEAM
}

AchievementManager::~AchievementManager() {
#ifdef STEAM
	SteamAPI_Shutdown();
	delete m_steamAchievements;
	delete m_cendricAchievements;

	for (auto kv : m_achievements) {
		delete kv.second;
	}
	m_achievements.clear();
#endif // STEAM
}

void AchievementManager::initAchievements() {
#ifdef STEAM
	for (int i = static_cast<int>(AchievementID::VOID) + 1; i < static_cast<int>(AchievementID::MAX); ++i) {
		auto achId = static_cast<AchievementID>(i);
		auto achName = getAchievementName(achId);

		if (contains(m_characterCore->getData().achievementsUnlocked, achName)) {
			continue;
		}

		if (contains(m_achievements, achName)) {
			continue;
		}

		auto achievement = ObjectFactory::Instance()->createAchievement(achId);
		if (achievement == nullptr) {
			continue;
		}

		m_achievements.insert({ achName, achievement });
	}
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
	m_characterCore->setAchievementUnlocked(achievement);
	m_steamAchievements->setAchievement(achievement.c_str());
#endif // STEAM
}
