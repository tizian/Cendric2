-- Routine for NPC "Jason" 

velocity = 40

loadRoutine = function(R, W)

    R:setTilePosition(7,26)
        
    R:setFacingRight()
    R:wait(5000)
    R:goToTile(7,25)
    R:goToTile(8.5,25)
    R:setFacingDown()
    R:wait(5000)
    R:goToTile(7,25)
    
    R:goToTile(7,26)

end	