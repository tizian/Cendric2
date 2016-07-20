-- Routine for NPC "Innkeeper"

velocity = 30

loadRoutine = function(R, W)

	R:setTilePosition(18,9.8)

	R:wait(5000)	
	R:goToTile(17,9.8)
	R:wait(5000)
	R:goToTile(19,9.8)

end	