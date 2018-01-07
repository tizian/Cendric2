#include "Steam/Achievements/WaypointAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_ALL_WAYPOINTS, WaypointAchievement)

bool WaypointAchievement::notifyCore(const CharacterCore* core) {
	return false;
}
