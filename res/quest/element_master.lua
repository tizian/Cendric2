main_quest = true
conditions = 
{
	"master_air",
	"master_earth"
}

markers = {
    {
		map = "res/map/gandria/gandria.tmx",
		position = {2475, 2225},
		step = 0
	},
    {
		map = "res/map/marshland/marshland.tmx",
		position = {1800, 850},
        npc = "npc_koray",
		step = 1
	},
	{
		map = "res/map/marshland/marshland.tmx",
		position = {400, 1575},
        npc = "npc_jonathan2",
		step = -1
	}
}

