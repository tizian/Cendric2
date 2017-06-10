-- Routine for NPC "Timo" 

velocity = 40

loadRoutine = function(R, W)
	
		R:setTilePosition(21, 13)
        
		R:goToTile(15,13)
        R:goToTile(15,11)
        R:goToTile(18,11)
        R:goToTile(18,7)
        R:goToTile(4,7)
        R:goToTile(4,4)
        R:goToTile(3.5,4.5)
        
        R:setFacingDown()
		R:wait(13000)
        
        R:goToTile(4,4) 
		R:goToTile(4,7)
        R:goToTile(18,7)
        R:goToTile(18,11)
        R:goToTile(21,11)
        R:goToTile(21,13)
end	