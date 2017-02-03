-- Routine for NPC "Jack" on the gandria map

velocity = 30

loadRoutine = function(R, W)

	if (not W:isConditionFulfilled("npc_jack","jack_back")) then
		R:setDisposed()
	end
	
	R:setTilePosition(71,43.8)
	
	R:setFacingDown()
	R:wait(10000)
	
	R:goToTile(71,47)
	R:goToTile(69,47)
	R:goToTile(69,46)
	R:goToTile(66,46)
	R:goToTile(66,43)
	R:goToTile(63,43)
	R:goToTile(63,52)
	R:goToTile(69,52)
	R:goToTile(69,47)
	R:goToTile(71,47)
	
	R:goToTile(71,43.8)
	
end	