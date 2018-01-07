#include "Steam/Achievements/NoDeathAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_NO_DEATH, NoDeathAchievement)

bool NoDeathAchievement::notifyCore(const CharacterCore* core) {
	return core->getData().deaths == 0;
}
