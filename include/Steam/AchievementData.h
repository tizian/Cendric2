#pragma once

#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }
struct AchievementData
{
	int m_achievementIdInt;
	const char* m_AchievementIdString;
	char m_name[128];
	char m_description[256];
	bool m_isAchieved;
	int m_iconImage;
};