-- Routine for NPC "Helena" 

velocity = 40

loadRoutine = function(R, W)
	
		R:setTilePosition(6,10)
		
		R:setFacingDown()
		R:wait(5000)
		R:goToTile(4,10)
		R:setFacingUp()
		R:wait(1000)
		R:setFacingDown()
		R:wait(5000)
		R:goToTile(7,10)
		R:setFacingDown()
		R:wait(4000)
		
end	