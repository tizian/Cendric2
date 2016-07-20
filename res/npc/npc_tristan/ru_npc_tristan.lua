-- Routine for NPC "Tristan"

velocity = 40

loadRoutine = function(R, W)

	R:setTilePosition(5,63)

	R:wait(6000)	
	R:goToTile(6,63)
	R:goToTile(5,63)

end	