#include "Steam/Achievements/ClericReputationAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_CLERIC_REP_70, ClericReputationAchievement)

bool ClericReputationAchievement::notifyCore(const CharacterCore* core) {
	return core->getReputation(FractionID::Cleric) >= 70;
}
