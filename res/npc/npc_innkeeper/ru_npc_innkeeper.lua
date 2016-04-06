-- Routine for NPC "Innkeeper"

velocity = 40

loadRoutine = function(R)

	R:setTilePosition(21,33)

	R:wait(5000)	
	R:goToTile(23,33)
	R:wait(5000)
	R:goToTile(23,33)

end	