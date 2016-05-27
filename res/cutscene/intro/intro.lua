-- lua script for cutscene intro
steps = {
	{
		texts = {
			{str="Darkness", time=2},

		},
		images = {
			{path="darkness.png", vel=0, angle=0}
		}
	},
	{
		texts = {
			{str="WhatHappened", time=2},
			{str="WhereAmI", 	 time=2}
		},
		images = {
			{path="forest_background.png", 	vel=0, 	angle=0},
			{path="forest_foreground.png", 	vel=10, angle=95},
			{path="forest_trees.png", 		vel=40, angle=80},
		}
	},
	{
		texts = {
			{str="Chapter1", time=2, centered=true}
		},
		images = {
			{path="darkness.png", vel=0, angle=0},
			{path = {en="chapter1_en.png", de="chapter1_de.png", ch="chapter1_de.png"}, vel=0, angle=0}
		},
		fadetime=1.0
	}
}

musicpath = "res/cutscene/intro/intro.ogg"

