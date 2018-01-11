#include "Steam/Achievements/PrisonBreakAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_PRISON_BREAK, PrisonBreakAchievement)

bool PrisonBreakAchievement::notify(const std::string& message) {
	if (message == "part1") {
		m_isPart1Done = true;
		return false;
	}

	if (message == "part2" && m_isPart1Done) {
		return true;
	}

	return false;
}
