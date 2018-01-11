#include "Steam/Achievements/LeverAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_MINIMALIST, LeverAchievement)

bool LeverAchievement::notify(const std::string& message) {
	if (message == "turn") {
		if (!m_hasStarted) return false;

		m_currentTurns++;
		return false;
	}

	if (message == "start") {
		m_currentTurns = 0;
		m_hasStarted = true;
	}

	if (message == "end") {
		m_hasStarted = false;
		if (m_currentTurns == 14 && m_hasStarted) {
			return true;
		}
	}

	return false;
}
