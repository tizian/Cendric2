-- Routine for NPC "Runa"

velocity = 50

loadRoutine = function(R, W)

	R:setTilePosition(18,57)
	
	R:goToTile(18,55)
    R:goToTile(17,55)
    R:goToTile(17,53)
    R:goToTile(9,53)
    R:goToTile(9,53.5)
    
    R:goToTile(4.5,53.5)
    R:goToTile(4.5,51.5)
    R:setFacingLeft()
    R:wait(1000)
    
    R:goToTile(4.5,55.5)
    R:setFacingRight()
    R:wait(1000)
    
    R:goToTile(7.5,53.5)
    R:goToTile(7.5,51.5)
    R:setFacingLeft()
    R:wait(1000)
    
    R:goToTile(7.5,55.5)
    R:setFacingRight()
    R:wait(1000)
    
    R:goToTile(7.5,53.5)
    R:goToTile(9,53.5)
    R:goToTile(9,53)
    R:goToTile(17,53)
    R:goToTile(17,55)
    R:goToTile(18,55)
    
	R:goToTile(18,57)
    R:wait(3000)

end	