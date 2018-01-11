#include "Steam/Achievements/GrimoireAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_NECRONOMICON, GrimoireAchievement)

bool GrimoireAchievement::notify(const std::string& message) {
	if (message == "skeleton") {
		m_summonedSkeletons++;
		return false;
	}

	if (message == "door") {
		m_isEnoughSkeletons = m_summonedSkeletons >= 10;
		return false;
	}

	if (message == "kill") {
		return m_isEnoughSkeletons;
	}

	return false;
}
