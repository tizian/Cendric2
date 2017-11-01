-- Routine for NPC "Robert" in Velius' Dungeon

velocity = 45

loadRoutine = function(R, W)
	
	R:setTilePosition(20,5)
	R:setReloadEnabled(false)
	
	R:setFacingDown()
	R:wait(4000)	
	R:goToTile(19,5)
	R:setFacingUp()
	R:wait(1000)
    R:goToTile(20,5)
	R:setFacingLeft()
	R:wait(1400)
    R:goToTile(21,5)
	R:setFacingUp()
	R:wait(1400)	
    R:goToTile(20,5)	
	
end	