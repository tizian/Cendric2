-- Routine for NPC "Luv"

velocity = 40

loadRoutine = function(R, W)

	if (W:isConditionFulfilled("npc_leeluv","gone")) then
		R:setDisposed()
	else
		R:setTilePosition(68,58)
		R:setFacingDown()
		R:setLooped(false)
	end

end	