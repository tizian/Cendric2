-- Routine for NPC "Sheep2"

velocity = 30

loadRoutine = function(R, W)

	R:setTilePosition(6,61)

	R:wait(3000)	
	R:goToTile(5,61)
	R:goToTile(6,61)

end	