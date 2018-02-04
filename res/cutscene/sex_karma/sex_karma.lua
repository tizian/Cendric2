-- lua script for cutscene "sex" with karma
mapid = "res/map/gandriabrothel/gandriabrothel.tmx"
worldx = 875
worldy = 320
steps = {
    {
		texts = {
            {str="Karma1", time=4, centered=true},
			{str="Karma2", time=5, centered=true}
		},
		images = {
			{path="black.png", vel=0, angle=0},
            {path="fg.png", vel=20, angle=0},
		},
		fadetime=1.0
	},
}