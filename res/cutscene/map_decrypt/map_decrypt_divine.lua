-- lua script for cutscene map_decrypt (divine)
mapid = "res/map/jonathanhome/jonathanhome.tmx"
worldx = 500	
worldy = 375
steps = {
	
	{
		texts = {
			{str="Jonathan:GotMap", time=3},
			{str="Jonathan:DivineMagic", time=4},
		},
		images = {
			{path="background.png", vel=0, angle=0},
			{path="john_map.png", vel=25, angle=340},
		}
	},
	
	{
		texts = {
			{str="Jonathan:Focus", time=7},
		},
		images = {
			{path="background.png", vel=0, angle=0},
			{path="john_spell.png", vel=10, angle=90},
			{path="john_magic.png", vel=13, angle=60},
			{path="cendric_divine.png", vel=50, angle=225},
			{path="cendric_spell.png", vel=10, angle=270},
		}
	},
	
	{
		texts = {
			{str="Jonathan:SpellBroken", time=4},
			{str="Jonathan:MapAndRunes", time=5},
			{str="Jonathan:MasterAllElements", time=7},
		},
		images = {
			{path="background.png", vel=0, angle=0},
			{path="map.png", vel=0, angle=0},
			{path="map_01.png", vel=40, angle=140},
			{path="map_02.png", vel=40, angle=20},
			{path="map_03.png", vel=40, angle=320},
		}
	},

	{
		texts = {
			{str="Chapter3", time=2, centered=true}
		},
		images = {
			{path="black.png", vel=0, angle=0},
			{path = {en="chapter3_en.png", de="chapter3_de.png", ch="chapter3_de.png", es="chapter3_es.png"}, vel=0, angle=0}
		},
		fadetime=1.0
	}
}