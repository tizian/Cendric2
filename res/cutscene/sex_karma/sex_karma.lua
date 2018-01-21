-- lua script for cutscene "sex" with karma
mapid = "res/map/gandriabrothel/gandriabrothel.tmx"
worldx = 875
worldy = 320
steps = {
    {
		texts = {
			{str="", time=1}
		},
		images = {
			{path="black.png", vel=0, angle=0}
		}
	},
	{
		texts = {
			{str="Karma1", time=2, centered=true}
		},
		images = {
			{path="black.png", vel=0, angle=0},
		},
		fadetime=1.0
	},
    {
		texts = {
			{str="Karma2", time=2, centered=true}
		},
		images = {
			{path="black.png", vel=0, angle=0},
		},
		fadetime=1.0
	},
}