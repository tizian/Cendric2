-- Routine for NPC "Andrew"

velocity = 20

loadRoutine = function(R)

	R:setTilePosition(7.3,8.6)
	
	R:goToTile(7.3,8.6)
	R:goToTile(7.3,8.7)
	R:wait(8000)
	R:goToTile(7.3,8.6)
	R:goToTile(6,8.6)
	R:goToTile(6,8.7)
	R:wait(8000)
	
end	