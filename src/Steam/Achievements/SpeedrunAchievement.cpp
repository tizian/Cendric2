#include "Steam/Achievements/SpeedrunAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_SPEEDRUN, SpeedrunAchievement)

bool SpeedrunAchievement::notifyCore(const CharacterCore* core) {
	return core->getData().timePlayed.asSeconds() < 10800.f;
}
