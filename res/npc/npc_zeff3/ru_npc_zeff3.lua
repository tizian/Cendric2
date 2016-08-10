-- Routine for NPC "Zeff" in Gandria
loadRoutine = function(R, W)

	if (not W:isConditionFulfilled("npc_zeff", "zeff_gone")) then
		R:setDisposed()
	end
	
	R:setTilePosition(9.5,59.7)
	
	R:goToTile(10.5,59.5)
	R:goToTile(10.5,59.7)
	R:wait(7000)
	R:goToTile(9.5,59.5)
	R:goToTile(9.5,59.7)
	R:wait(7000)

end	