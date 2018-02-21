-- lua script for cutscene velius
steps = {
    {
		texts = {
			{str="Logan:Ah", time=3},
            {str="Logan:Tried", time=5},
		},
		images = {
			{path="1_1.png", vel=0, angle=0},
            {path="1_2.png", vel=15, angle=270},
            {path="1_3.png", vel=100, angle=90},
            {path="1_4.png", vel=15, angle=270},
            {path="1_5.png", vel=0, angle=0},
            {path="1_6.png", vel=3, angle=0},
            {path="1_7.png", vel=0, angle=0}
		}
	},
    {
		texts = {
			{str="Velius:Enough", time=3},
            {str="Velius:Nearly", time=5},
            {str="Velius:Now", time=6},
            {str="Velius:Need", time=5}
		},
		images = {
			{path="1_1.png", vel=0, angle=0},
            {path="2_2.png", vel=8, angle=0},
            {path="2_3.png", vel=8, angle=0},
            {path="2_5.png", vel=0, angle=0},
            {path="2_6.png", vel=3, angle=0},
            {path="2_7.png", vel=0, angle=0}
		}
	},
    {
		texts = {
			{str="Velius:Die", time=8}
		},
		images = {
			{path="1_1.png", vel=0, angle=0},
            {path="3_2.png", vel=0, angle=0},
            {path="3_3.png", vel=20, angle=250},
            {path="3_4.png", vel=70, angle=250},
            {path="3_5.png", vel=55, angle=250},
            {path="3_6.png", vel=40, angle=250},
            {path="3_7.png", vel=20, angle=250},
            {path="2_5.png", vel=0, angle=0},
            {path="2_6.png", vel=3, angle=0},
            {path="2_7.png", vel=0, angle=0}
		}
	}
}