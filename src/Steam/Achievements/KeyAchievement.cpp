#include "Steam/Achievements/KeyAchievement.h"
#include "CharacterCore.h"
#include "Registrar.h"

REGISTER_ACHIEVEMENT(AchievementID::ACH_ALL_KEYS, KeyAchievement)

bool KeyAchievement::notifyCore(const CharacterCore* core) {
	auto const& items = core->getData().items;

	return
		contains(items, std::string("ke_tower_4")) &&
		contains(items, std::string("ke_tower_3")) &&
		contains(items, std::string("ke_tower_2")) &&
		contains(items, std::string("ke_tower_1")) &&
		contains(items, std::string("ke_smallsilver")) &&
		contains(items, std::string("ke_rhendal")) &&
		contains(items, std::string("ke_tavernbasement")) &&
		contains(items, std::string("ke_jonathanbasement")) &&
		contains(items, std::string("ke_syrahbasement")) &&
		contains(items, std::string("ke_thiefguild")) &&
		contains(items, std::string("ke_sewerkey")) &&
		contains(items, std::string("ke_lighthouse")) &&
		contains(items, std::string("ke_forgottenpassage")) &&
		contains(items, std::string("ke_cathedral")) &&
		contains(items, std::string("ke_tower"));
}
