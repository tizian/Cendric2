#pragma once

#include "global.h"

class SteamAchievements;
class CharacterCore;
class Achievement;
struct AchievementData;

class AchievementManager final {
public:
	AchievementManager();
	~AchievementManager();

	void setCore(CharacterCore* core);

	void notifyAchievement(const std::string& achievement, const std::string& tag, const std::string& message);
	void notifyAchievementCore(const std::string& achievement);
	void unlockAchievement(const std::string& achievement);

private:
	void initAchievements();
	void clearAchievements();

private:
	CharacterCore* m_characterCore = nullptr;
	SteamAchievements* m_steamAchievements = nullptr;
	AchievementData* m_cendricAchievements = nullptr;
	std::map<std::string, Achievement*> m_achievements;
};