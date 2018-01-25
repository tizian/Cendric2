#include "Steam/Achievement.h"
#include "ObjectFactory.h"
#include "Registrar.h"

#define ACH_CONST []() -> Achievement* { return new Achievement(); }

// define the achievements that have no special subclasses (no data)
static std::vector<std::pair<AchievementID, AchievementConstructor>> achievements = {
	{ ACH_SAVE_ZEFF, ACH_CONST },
	{ ACH_KILL_ZEFF, ACH_CONST },
	{ ACH_KILL_KHAJAG, ACH_CONST },
	{ ACH_KILL_ELYSIA, ACH_CONST },
	{ ACH_KILL_JANUS, ACH_CONST },
	{ ACH_KILL_MERCENARIES, ACH_CONST },
	{ ACH_FRIEND_YASHA, ACH_CONST },
	{ ACH_KILL_YASHA, ACH_CONST },
	{ ACH_KILL_ROYAL_GUARDS, ACH_CONST },
	{ ACH_KILL_VELIUS, ACH_CONST },

	{ ACH_ETOZ_1, ACH_CONST },
	{ ACH_ETOZ_2, ACH_CONST },
	{ ACH_ETOZ_3, ACH_CONST },
	{ ACH_ETOZ_4, ACH_CONST },
	
	{ ACH_LEEROY, ACH_CONST },
	{ ACH_TEARSTAFF, ACH_CONST },
	{ ACH_CHICKEN_SHOCKER, ACH_CONST },
	{ ACH_CRIMINAL, ACH_CONST },
	{ ACH_SAVE_ALL_MAGES, ACH_CONST },
	{ ACH_KILL_ALL_MAGES, ACH_CONST },
	{ ACH_MERCENARY_ORDER, ACH_CONST },
};

static Registrar registrar(achievements);