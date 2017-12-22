// Defining our achievements
enum AchievementID
{
	ACH_WIN_ONE_GAME = 0,
	ACH_WIN_100_GAMES = 1,
	ACH_TRAVEL_FAR_ACCUM = 2,
	ACH_TRAVEL_FAR_SINGLE = 3,
};

AchievementData g_achievements[] =
{
	_ACH_ID( ACH_WIN_ONE_GAME, "Winner" ),
	_ACH_ID( ACH_WIN_100_GAMES, "Champion" ),
	_ACH_ID( ACH_TRAVEL_FAR_ACCUM, "Interstellar" ),
	_ACH_ID( ACH_TRAVEL_FAR_SINGLE, "Orbiter" ),
};