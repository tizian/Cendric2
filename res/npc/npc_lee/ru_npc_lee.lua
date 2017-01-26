-- Routine for NPC "Lee"

velocity = 40

loadRoutine = function(R, W)

	if (W:isConditionFulfilled("npc_leeluv","gone")) then
		R:setDisposed()
	else
		R:setTilePosition(62,54)
		R:setFacingDown()
		R:setLooped(false)
	end

end	