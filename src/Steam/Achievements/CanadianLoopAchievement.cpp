#include "Steam/Achievements/CanadianLoopAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_CANADIAN_LOOP, CanadianLoopAchievement)

bool CanadianLoopAchievement::notify(const std::string& message) {
	if (message != "loop") return;	
	
	return ++m_loopCount >= 10;
}
