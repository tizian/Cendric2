-- Routine for NPC "Citizen F 1"

velocity = 40

loadRoutine = function(R, W)

		R:setTilePosition(36,21)
		
		R:goToTile(36,8)
		R:goToTile(42,8)
		R:goToTile(42,7)
		R:setFacingLeft()
		R:wait(5000)
		R:goToTile(42,8)
		R:goToTile(36,8)
		R:goToTile(36,15)
		R:goToTile(37,15)
		R:setFacingDown()
		R:wait(5000)
		R:goToTile(36,15)
		R:goToTile(36,21)
		R:goToTile(53,21)
		R:goToTile(53,18)
		R:goToTile(51,18)
		R:goToTile(51,6)
		R:goToTile(60,6)
		R:goToTile(60,18)
		R:goToTile(53,18)
		R:goToTile(53,21)
		R:goToTile(49,21)
		R:goToTile(49,25)
		R:goToTile(42,25)
		R:goToTile(42,21)
		
		R:goToTile(36,21)

end	