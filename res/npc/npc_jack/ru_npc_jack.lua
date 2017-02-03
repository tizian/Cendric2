-- Routine for NPC "Jack"

loadRoutine = function(R, W)

	if (W:isConditionFulfilled("npc_jack","jack_back")) then
		R:setReloadEnabled(false)
		R:setTilePosition(9,5.2)
		
		R:goToTile(12,5.2)
		R:goToTile(12,17)
		R:setDisposed()
		return
	end

	R:setTilePosition(9,5.2)
	R:setFacingRight()
	R:setLooped(false)
	R:setReloadEnabled(true)
	
end	