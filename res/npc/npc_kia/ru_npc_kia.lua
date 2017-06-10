-- Routine for NPC "Kia" 

velocity = 50

loadRoutine = function(R, W)
	
		R:setTilePosition(17,4.5)
        
        R:setFacingDown()
        R:wait(7000)
        

		R:goToTile(17,5)
        R:goToTile(16,5)
        R:goToTile(16,6)
        R:goToTile(16,7)
        R:goToTile(10,7)
        R:goToTile(10,10)
        R:goToTile(4,10)
		R:setFacingRight()
		R:wait(3000)
        R:goToTile(10,10)
        R:goToTile(10,7)
        R:goToTile(12,7)
        R:goToTile(12,3.7)
        R:setFacingDown()
		R:wait(10000)
        R:goToTile(12,7)
        R:goToTile(16,7)
        R:goToTile(16,6)
        R:goToTile(16,5)
        R:goToTile(17,5)
        R:goToTile(17,4.5)
		
end	