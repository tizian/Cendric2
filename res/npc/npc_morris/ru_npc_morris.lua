-- Routine for NPC "Morris"

loadRoutine = function(R, W)

	R:setTilePosition(5,11)
	
	R:wait(3000)
	R:goToTile(6,11)
	R:wait(2000)
	R:goToTile(6,12)
	R:wait(2000)
	R:goToTile(6,11)
	R:wait(500)
	R:goToTile(5,11)

end	