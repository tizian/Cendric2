-- Routine for NPC "Morton"

loadRoutine = function(R, W)

	R:setTilePosition(11,9)
	
	R:setFacingDown()
	R:wait(8000)
	R:goToTile(13,9)
	R:goToTile(13,8)
	R:goToTile(17,8)
	R:goToTile(17,7)
	R:setFacingLeft()
	R:wait(8000)
	R:goToTile(17,8)
	R:goToTile(15,8)
	R:goToTile(15,9)
	R:setFacingRight()
	R:wait(8000)
	R:goToTile(15,8)
	R:goToTile(13,8)
	R:goToTile(13,9)
	R:goToTile(11,9)
	
end	