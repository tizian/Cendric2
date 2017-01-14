-- Routine for NPC "Citizen 1"

velocity = 40

loadRoutine = function(R, W)

		R:setTilePosition(23,35)
		
		R:goToTile(23,39)
		R:goToTile(34,39)
		R:goToTile(34,52)
		R:goToTile(22,52)
		R:goToTile(22,35)
		R:goToTile(19,35)
		R:goToTile(19,22)
		R:setFacingDown()
		R:wait(3000)
		R:goToTile(19,35)
		
		R:goToTile(23,35)

end	