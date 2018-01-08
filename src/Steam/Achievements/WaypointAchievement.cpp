#include "Steam/Achievements/WaypointAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_ALL_WAYPOINTS, WaypointAchievement)

bool WaypointAchievement::notifyCore(const CharacterCore* core) {
	auto const& waypoints = core->getData().waypointsUnlocked;
	// meadows + gandria + sewers + marshland + highland
	const size_t totalWaypoints = 10 + 18 + 6 + 6 + 6;

	size_t waypointCount = 0;
	for (auto wp : waypoints) {
		waypointCount += wp.second.size();
	}

	return waypointCount == totalWaypoints;
}
