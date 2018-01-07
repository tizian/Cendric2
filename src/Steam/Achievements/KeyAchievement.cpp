#include "Steam/Achievements/KeyAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_ALL_KEYS, KeyAchievement)

bool KeyAchievement::notifyCore(const CharacterCore* core) {
	return false;
}
