-- Routine for NPC "Leeroy"

loadRoutine = function(R, W)

    if (W:isConditionFulfilled("default","swampbridge_open")) then
		R:setDisposed()
        return
	end

	if (not W:isConditionFulfilled("npc_leeroy","talked")) then
		R:setTalkingActive(true)
	end

	R:setTilePosition(27,9)
	R:setFacingUp()
	R:setReloadEnabled(true)
	R:setLooped(false)
	
end	