-- Routine for NPC "Tristan"

velocity = 40

loadRoutine = function(R)

	R:setTilePosition(5,63)

	R:wait(5000)	
	R:goToTile(6,63)
	R:goToTile(5,63)

end	