-- Routine for NPC "Koray" in Velius' Dungeon

velocity = 50

loadRoutine = function(R, W)
	
	R:setTilePosition(20,12)
	R:setReloadEnabled(false)
	
	R:setFacingLeft()
	R:wait(3500)	
	R:goToTile(19,12)
	R:setFacingDown()
	R:wait(1700)
    R:goToTile(21,12)
	R:setFacingRight()
	R:wait(1800)	
    R:goToTile(20,12)	
	
end	