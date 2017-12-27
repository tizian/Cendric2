#include "Steam/Achievements/GoldAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_GOLD_1000, GoldAchievement)

bool GoldAchievement::notifyCore(const CharacterCore* core) {
	return core->hasItem("gold", 1000);
}
