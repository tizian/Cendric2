-- Routine for NPC "Ingid"

velocity = 40

loadRoutine = function(R, W)

	R:setTilePosition(46,32.5)

	R:wait(5000)	
	R:goToTile(46,33.6)
	R:goToTile(47,33.6)
	R:goToTile(47,33.5)
	R:wait(5000)
	R:goToTile(48,33.5)
	R:goToTile(48,32.5)
	R:goToTile(47.9,32.5)
	R:wait(5000)
	R:goToTile(48,33.5)
	R:goToTile(46,33.5)
	R:goToTile(45.9,32.5)
	R:goToTile(46,32.5)
	
end	