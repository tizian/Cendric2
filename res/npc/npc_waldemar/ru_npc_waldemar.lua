-- Routine for NPC "Waldemar"

loadRoutine = function(R, W)

	R:setTilePosition(16, 15.5)
	
	R:setFacingUp()
	R:wait(6000)

	R:goToTile(17, 15.5)
	R:setFacingUp()
	R:wait(4000)
	
	R:goToTile(17,16)
	R:goToTile(11,16)
	R:goToTile(11,12)
	R:goToTile(3,12)
	R:goToTile(3,10.5)
	R:setFacingUp()
	R:wait(7000)
	
	R:goToTile(3,12)
	R:goToTile(22,12)
	R:goToTile(22,16)
	R:goToTile(19,16)
	R:goToTile(19,15.5)
	R:setFacingUp()
	R:wait(10000)
	
	R:goToTile(19,16)
	R:goToTile(22,16)
	R:goToTile(22,12)
	R:goToTile(11,12)
	R:goToTile(11,16)
	R:goToTile(16,16)
	
	R:goToTile(16, 15.5)
	
end	