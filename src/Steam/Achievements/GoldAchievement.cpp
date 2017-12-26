#include "Steam/Achievements/GoldAchievement.h"
#include "CharacterCore.h"

bool GoldAchievement::notifyCore(const CharacterCore* core) {
	return core->hasItem("gold", 1000);
}
