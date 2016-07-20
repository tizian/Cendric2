-- Routine for NPC "Sheep1"

velocity = 20

loadRoutine = function(R, W)

	R:setTilePosition(4,60)

	R:wait(5000)	
	R:goToTile(4,61)
	R:goToTile(4,60)

end	