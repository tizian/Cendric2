#include "Steam/Achievement.h"
#include "ObjectFactory.h"
#include "Registrar.h"

#define ACH_CONST []() -> Achievement* { return new Achievement(); }

// define the achievements that have no special subclasses (no data)
static std::vector<std::pair<AchievementID, AchievementConstructor>> achievements = {
	{ AchievementID::ACH_SAVE_ZEFF, ACH_CONST },
	{ AchievementID::ACH_KILL_ZEFF, ACH_CONST },
	{ AchievementID::ACH_KILL_KHAJAG, ACH_CONST },
	{ AchievementID::ACH_KILL_ELYSIA, ACH_CONST },
	{ AchievementID::ACH_KILL_MERCENARIES, ACH_CONST },
	{ AchievementID::ACH_KILL_ALASTOR, ACH_CONST },
	{ AchievementID::ACH_FRIEND_YASHA, ACH_CONST },
	{ AchievementID::ACH_KILL_YASHA, ACH_CONST },
	{ AchievementID::ACH_KILL_ROYAL_GUARDS, ACH_CONST },
	{ AchievementID::ACH_KILL_VELIUS, ACH_CONST },

	{ AchievementID::ACH_ETOZ_1, ACH_CONST },
	{ AchievementID::ACH_ETOZ_2, ACH_CONST },
	{ AchievementID::ACH_ETOZ_3, ACH_CONST },
	{ AchievementID::ACH_ETOZ_4, ACH_CONST },

	{ AchievementID::ACH_CHICKEN_SHOCKER, ACH_CONST },
	{ AchievementID::ACH_LEEROY, ACH_CONST },
	{ AchievementID::ACH_TEARSTAFF, ACH_CONST },
};

static Registrar registrar(achievements);