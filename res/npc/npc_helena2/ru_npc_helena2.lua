-- Routine for NPC "Helena" outside of her inn.

velocity = 40

loadRoutine = function(R, W)

	if (W:isConditionFulfilled("npc_helena2","talked")) then
		R:setTilePosition(28,51)
		R:goToTile(28,49)
		R:setDisposed()
	else
		R:setTalkingActiveForce(true)
		R:setReloadEnabled(true)
		R:setTilePosition(28,51)
		R:setFacingDown()
	end

end	