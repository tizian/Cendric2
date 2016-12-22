-- Routine for NPC "Zeff" in Gandria
loadRoutine = function(R, W)

	if (not W:isConditionFulfilled("npc_zeff", "zeff_gone")) then
		R:setDisposed()
		return
	end
	
	R:setTilePosition(9.5,49.7)
	
	R:goToTile(10.5,49.7)
	R:goToTile(10.5,49.5)
	R:wait(7000)
	R:goToTile(9.5,49.5)
	R:goToTile(9.5,49.7)
	R:wait(7000)

end	