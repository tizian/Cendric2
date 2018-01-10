#include "Steam/Achievements/MercenariesOrderAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_MERCENARY_ORDER, MercenariesOrderAchievement)

bool MercenariesOrderAchievement::notify(const std::string& message) {
	if (message == "morgiana" && m_correctOrderState == 0) {
		m_correctOrderState = 1;
		return false;
	}

	if (message == "roy" && m_correctOrderState == 1) {
		m_correctOrderState = 2;
		return false;
	}

	if (message == "jeremy" && m_correctOrderState == 2) {
		return true;
	}

	return false;
}
