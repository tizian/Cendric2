-- Routine for NPC "Lloyd"

loadRoutine = function(R, W)

	R:setTilePosition(19,2)
	
	R:setFacingDown()
	R:wait(10000)

	R:goToTile(14,2)
	R:setFacingUp()
	R:wait(7000)
	R:goToTile(14,4)
	R:goToTile(20,4)
	R:goToTile(20,6)
	R:goToTile(20.5,6)
	R:wait(5000)
	R:goToTile(20.5,5)
	R:goToTile(21,5)
	R:goToTile(21,2)
	R:goToTile(19,2)

end	