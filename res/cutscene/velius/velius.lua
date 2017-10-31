-- lua script for cutscene velius
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
			{str="Chapter5", time=2, centered=true}
		},
		images = {
			{path="black.png", vel=0, angle=0},
			{path = {en="chapter5_en.png", de="chapter5_de.png", ch="chapter5_de.png", es="chapter5_es.png"}, vel=0, angle=0}
		},
		fadetime=1.0
	}
}