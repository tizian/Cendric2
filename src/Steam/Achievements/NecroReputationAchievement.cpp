#include "Steam/Achievements/NecroReputationAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_NECRO_REP_70, NecroReputationAchievement)

bool NecroReputationAchievement::notifyCore(const CharacterCore* core) {
	return core->getReputation(FractionID::Necromancer) >= 70;
}
