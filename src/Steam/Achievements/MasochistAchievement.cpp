#include "Steam/Achievements/MasochistAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_MASOCHIST, MasochistAchievement)

bool MasochistAchievement::notifyCore(const CharacterCore* core) {
	return false;
}
