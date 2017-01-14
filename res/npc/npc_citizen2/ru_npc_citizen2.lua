-- Routine for NPC "Citizen 2"

velocity = 40

loadRoutine = function(R, W)

		R:setTilePosition(68,53)
		
		R:goToTile(50,53)
		R:goToTile(50,59)
		R:goToTile(34,59)
		R:goToTile(34,52)
		R:goToTile(50,52)
		R:goToTile(50,43)
		R:goToTile(58,43)
		R:goToTile(58,52)
		R:goToTile(63,52)
		R:goToTile(63,43)
		R:goToTile(66,43)
		R:goToTile(58,46)
		R:goToTile(69,46)
		R:goToTile(69,53)
		R:setFacingDown()
		R:wait(3000)
		
		R:goToTile(68,53)

end	