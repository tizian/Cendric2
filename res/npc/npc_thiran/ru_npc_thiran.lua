-- Routine for NPC "Thiran"

loadRoutine = function(R, W)

	R:setTilePosition(11,2)
	
	R:setFacingDown()
	R:wait(16000)

	R:goToTile(9,2)
	R:setFacingUp()
	R:wait(3000)
	R:goToTile(17,2)
	R:setFacingUp()
	R:wait(3000)
	R:goToTile(16,2)
	R:goToTile(16,11)
	R:goToTile(16.5,11)
	R:wait(5000)
	R:goToTile(16,11)
	R:goToTile(16,2)
	R:goToTile(11,2)

end	