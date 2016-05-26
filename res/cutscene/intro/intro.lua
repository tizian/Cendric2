-- lua script for cutscene intro
steps = 
{
	{
		{
			{"Darkness", 2}
		},
		{
			{"darkness.png", 0, 0}
		}
	},
	{
		{
			{"WhatHappened", 5},
			{"WhereAmI", 5}
		},
		{
			{"forest_background.png", 0, 0},
			{"forest_foreground.png", 10, 95},
			{"forest_trees.png", 40, 80},
		}
	},
	{
		{
			{"", 3}
		},
		{
			{"darkness.png", 0, 0},
			{"chapter1.png", 0, 0}
		}
	}
}

musicpath = "res/cutscene/intro/intro.ogg"

