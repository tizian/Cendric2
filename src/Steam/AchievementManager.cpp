#include "Steam/AchievementManager.h"
#include "CharacterCore.h"
#include "ObjectFactory.h"
#include "Steam/Achievement.h"
#include "Steam/CendricAchievements.h"

#ifdef STEAM
#include "Steam/SteamAchievements.h"
#endif

AchievementManager* g_achievementManager;

AchievementManager::AchievementManager() {
	m_cendricAchievements = createAchievementsArray();
#ifdef STEAM
	const bool isSuccess = SteamAPI_Init();

	if (isSuccess)
	{
		m_steamAchievements = new SteamAchievements(m_cendricAchievements, static_cast<int>(AchievementID::MAX) - 1);
	}
#endif // STEAM
}

AchievementManager::~AchievementManager() {
#ifdef STEAM
	SteamAPI_Shutdown();
	delete m_steamAchievements;
	
#endif // STEAM
	clearAchievements();
	delete m_cendricAchievements;
}

void AchievementManager::setCore(CharacterCore* core) {
	m_characterCore = core;
	initAchievements();
}

void AchievementManager::initAchievements() {
	if (!m_characterCore) return;

	clearAchievements();
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
}

void AchievementManager::clearAchievements() {
	for (auto kv : m_achievements) {
		delete kv.second;
	}
	m_achievements.clear();
}

void AchievementManager::notifyAchievement(AchievementID achievementId, const std::string& message) {
	if (!contains(AchievementNames, achievementId)) return;
	notifyAchievement(AchievementNames.at(achievementId), message);
}

void AchievementManager::notifyAchievement(const std::string& achievement, const std::string& message) {
	if (!m_characterCore) return;
	if (!contains(m_achievements, achievement)) return;

	if (m_achievements.at(achievement)->notify(message)) {
		unlockAchievement(achievement);
	}
}

void AchievementManager::notifyAchievementCore(AchievementID achievementId) {
	if (!contains(AchievementNames, achievementId)) return;
	notifyAchievementCore(AchievementNames.at(achievementId));
}

void AchievementManager::notifyAchievementCore(const std::string& achievement) {
	if (!m_characterCore) return;
	if (!contains(m_achievements, achievement)) return;

	if (m_achievements.at(achievement)->notifyCore(m_characterCore)) {
		unlockAchievement(achievement);
	}
}

void AchievementManager::unlockAchievement(AchievementID achievementId) {
	if (!contains(AchievementNames, achievementId)) return;
	unlockAchievement(AchievementNames.at(achievementId));
}

void AchievementManager::unlockAchievement(const std::string& achievement) {
	if (!m_characterCore) return;
	if (!m_characterCore->getData().hashValid) return;
	if (!contains(m_achievements, achievement)) return;

	auto it = m_achievements.find(achievement);
	delete (*it).second;
	m_achievements.erase(it);
	m_characterCore->setAchievementUnlocked(achievement);
#ifdef STEAM
	if (m_steamAchievements)
		m_steamAchievements->setAchievement(achievement.c_str());
#endif // STEAM
}
