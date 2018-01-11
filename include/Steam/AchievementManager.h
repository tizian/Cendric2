#pragma once

#include "global.h"
#include "Achievement.h"

class SteamAchievements;
class CharacterCore;
class Achievement;
struct AchievementData;

class AchievementManager final {
public:
	AchievementManager();
	~AchievementManager();

	void setCore(CharacterCore* core);

	void notifyAchievement(AchievementID achievementId, const std::string& message);
	void notifyAchievement(const std::string& achievement, const std::string& message);
	void notifyAchievementCore(AchievementID achievementId);
	void notifyAchievementCore(const std::string& achievement);
	void notifyAchievementScreen(AchievementID achievementId, const Screen* screen);
	void notifyAchievementScreen(const std::string& achievement, const Screen* screen);
	void unlockAchievement(const std::string& achievement);
	void unlockAchievement(AchievementID achievementId);

private:
	void initAchievements();
	void clearAchievements();

private:
	CharacterCore* m_characterCore = nullptr;
	SteamAchievements* m_steamAchievements = nullptr;
	AchievementData* m_cendricAchievements = nullptr;
	std::map<std::string, Achievement*> m_achievements;
};