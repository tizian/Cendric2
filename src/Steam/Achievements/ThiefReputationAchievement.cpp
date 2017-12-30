#include "Steam/Achievements/ThiefReputationAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_THIEF_REP_70, ThiefReputationAchievement)

bool ThiefReputationAchievement::notifyCore(const CharacterCore* core) {
	return core->getReputation(FractionID::Thief) >= 70;
}
