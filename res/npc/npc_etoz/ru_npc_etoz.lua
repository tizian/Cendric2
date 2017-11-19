-- Routine for NPC "Etoz the Mighty" in the Sewers

velocity = 40

loadRoutine = function(R, W)

	R:setTilePosition(25,5)

    R:setFacingLeft()
	R:wait(2000)	
	R:goToTile(33,5)
	R:goToTile(33,3)
	R:goToTile(34,3)
    R:setFacingDown()
	R:wait(4000)
    R:goToTile(34,5)
	R:goToTile(37,5)
    R:setFacingUp()
	R:wait(5000)
    R:goToTile(33,5)
	R:goToTile(33,10)
    R:goToTile(32,10)
    R:setFacingUp()
	R:wait(3000)
    R:goToTile(33,10)
    R:goToTile(33,5)
	R:goToTile(25,5)

end	