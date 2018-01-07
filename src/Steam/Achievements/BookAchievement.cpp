#include "Steam/Achievements/BookAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_ALL_BOOKS, BookAchievement)

bool BookAchievement::notifyCore(const CharacterCore* core) {
	return false;
}
