#include "Steam/SteamAchievements.h"
#include "Logger.h"

SteamAchievements::SteamAchievements(AchievementData* Achievements, int achievementCount) :
	m_callbackUserStatsReceived(this, &SteamAchievements::onUserStatsReceived),
	m_callbackUserStatsStored(this, &SteamAchievements::onUserStatsStored),
	m_callbackAchievementStored(this, &SteamAchievements::onAchievementStored)
{
	m_isInitialized = false;
	m_appID = SteamUtils()->GetAppID();
	m_achievements = Achievements;
	m_achievementCount = achievementCount;
	requestStats();
}

SteamAchievements::~SteamAchievements() {
}

bool SteamAchievements::requestStats()
{
	// Is Steam loaded? If not we can't get stats.
	if (!SteamUserStats() || !SteamUser())
	{
		return false;
	}
	// Is the user logged on?  If not we can't get stats.
	if (!SteamUser()->BLoggedOn())
	{
		return false;
	}
	// Request user stats.
	return SteamUserStats()->RequestCurrentStats();
}

bool SteamAchievements::setAchievement(const char* ID)
{
	// Have we received a call back from Steam yet?
	if (m_isInitialized)
	{
		SteamUserStats()->SetAchievement(ID);
		return SteamUserStats()->StoreStats();
	}
	// If not then we can't set achievements yet
	return false;
}

void SteamAchievements::onUserStatsReceived(UserStatsReceived_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_appID == pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			g_logger->logInfo("SteamAchievements", "Received stats and achievements from Steam");
			m_isInitialized = true;

			// load achievements
			for (int i = 0; i < m_achievementCount; ++i)
			{
				AchievementData& achievement = m_achievements[i];

				SteamUserStats()->GetAchievement(achievement.m_AchievementIdString, &achievement.m_isAchieved);
				_snprintf(achievement.m_name, sizeof(achievement.m_name), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(achievement.m_AchievementIdString,
						"name"));
				_snprintf(achievement.m_description, sizeof(achievement.m_description), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(achievement.m_AchievementIdString,
						"desc"));
			}
		}
		else
		{
			g_logger->logError("SteamAchievements", "RequestStats - failed");
		}
	}
}

void SteamAchievements::onUserStatsStored(UserStatsStored_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_appID == pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			g_logger->logInfo("SteamAchievements", "Stored stats for Steam");
		}
		else
		{
			g_logger->logError("SteamAchievements", "StatsStored - failed");
		}
	}
}

void SteamAchievements::onAchievementStored(UserAchievementStored_t *pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_appID == pCallback->m_nGameID)
	{
		g_logger->logInfo("SteamAchievements", "Achievement stored.");
	}
}

