-- Routine for NPC "Don Diego" 

velocity = 60

loadRoutine = function(R, W)
	
		R:setTilePosition(4,5)

		R:goToTile(4,7)
        R:goToTile(8,7)
        R:goToTile(8,5)
        R:goToTile(9,5)
        R:goToTile(9,4.5)
        R:goToTile(7.5,4.5)
		R:setFacingDown()
		R:wait(5000)
        R:goToTile(7,5)
        R:goToTile(8,5)
        R:goToTile(8,7)
        R:goToTile(10,7)
        R:goToTile(10,10)
        R:goToTile(12,10)
        R:goToTile(13,10)
        R:goToTile(13,11)
        R:goToTile(13,13)
        R:goToTile(10,13)
        R:goToTile(10,12)
        R:goToTile(3,12)
		R:setFacingUp()
		R:wait(5000)
		R:goToTile(10,12)
        R:goToTile(10,7)
        R:goToTile(4,7)
        R:goToTile(4,5)
		
end	