#include "Steam/Achievements/BookAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_ALL_BOOKS, BookAchievement)

bool BookAchievement::notifyCore(const CharacterCore* core) {
	auto const& books = core->getData().booksRead;

	return
		contains(books, std::string("bo_rhendalmagic")) &&
		contains(books, std::string("bo_schoolsofmagic")) &&
		contains(books, std::string("bo_potions")) &&
		contains(books, std::string("do_elderdiary")) &&
		contains(books, std::string("do_antimagic"));
}
