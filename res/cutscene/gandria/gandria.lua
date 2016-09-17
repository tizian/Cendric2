-- lua script for cutscene gandria
mapid = "res/map/gandria/gandria.tmx"
worldx = 215
worldy = 3225
steps = {
	{
		texts = {
			{str="Guard:OpenTheGates", time=10},
			{str="", time=7},
		},
		images = {
			{path="1_background.png", vel=0, angle=0},
			{path="1_doorbg.png", vel=90, angle=180},
			{path="1_gate.png", vel=30, angle=180},
			{path="1_sea.png", vel=40, angle=180},
			{path="1_ship.png", vel=50, angle=-139},
			{path="1_city2.png", vel=60, angle=180},
			{path="1_city2_smoke.png", vel=50, angle=177},
			{path="1_city1.png", vel=70, angle=180},
			{path="1_city1_smoke.png", vel=55, angle=177},
			{path="1_seagulls.png", vel=100, angle=100},
			{path="1_door.png", vel=90, angle=180},
		}
	},
	
	{
		texts = {
			{str="", time=1},
			{str="Velius:Well", time=4},
			{str="Velius:SecurityMeasures", time=8},
		},
		images = {
			{path="2_background.png", vel=0, angle=0},
			{path="2_velius.png", vel=30, angle=90},
			{path="2_magic.png", vel=35, angle=80},
			{path="2_flames1.png", vel=24, angle=60},
			{path="2_flames2.png", vel=22, angle=60},
			{path="2_pillars.png", vel=20, angle=90},
		}
	},
	
	{
		texts = {
			{str="Chapter2", time=2, centered=true}
		},
		images = {
			{path="black.png", vel=0, angle=0},
			{path = {en="chapter2_en.png", de="chapter2_de.png", ch="chapter2_de.png"}, vel=0, angle=0}
		},
		fadetime=1.0
	}
}
