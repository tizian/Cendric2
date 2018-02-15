-- Routine for NPC "Stephen" in the barracks

loadRoutine = function(R, W)

	if (not W:isConditionFulfilled("npc_steven", "talked")) then 
	
		R:setDisposed()
		return
    
    end
	
	R:setTilePosition(5,9)
	R:setReloadEnabled(true)

	R:goToTile(5,9)
	R:goToTile(5,11)
	R:goToTile(11,11)
	R:wait(5000)
	R:goToTile(5,11)
	R:goToTile(5,3)
	R:wait(10000)

end	