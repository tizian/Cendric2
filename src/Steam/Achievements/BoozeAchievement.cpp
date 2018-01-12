#include "Steam/Achievements/BoozeAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_BOOZE, BoozeAchievement)

bool BoozeAchievement::notify(const std::string& message) {
	if (message == "start") {
		m_isStarted = true;
		m_boozeAmount = 0;
		return false;
	}

	if (!m_isStarted) return false;

	if (message == "end") {
		m_isStarted = false;
		return m_boozeAmount >= 12;
	}

	if (message == "fo_beer" || message == "fo_wine" || message == "fo_rum") {
		m_boozeAmount++;
	}

	return false;
}
