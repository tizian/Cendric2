-- lua script for cutscene gandria
mapid = "res/map/meadows/meadows.tmx"
worldx = 3360
worldy = 3030
steps = {
	{
		texts = {
			{str="Chapter2", time=2, centered=true}
		},
		images = {
			{path="black.png", vel=0, angle=0},
			{path = {en="chapter2_en.png", de="chapter2_de.png", ch="chapter2_de.png"}, vel=0, angle=0}
		},
		fadetime=1.0
	},
	{
		texts = {
			{str="", time=3, centered=true}
		},
		images = {
			{path="black.png", vel=0, angle=0},
			{path = {en="thanks_en.png", de="thanks_de.png", ch="thanks_ch.png"}, vel=0, angle=0}
		},
		fadetime=1.0
	}
}
