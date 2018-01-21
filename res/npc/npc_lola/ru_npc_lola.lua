-- Routine for NPC "Lola"

loadRoutine = function(R, W)

    if (W:isConditionFulfilled("npc_lola", "sex") and not W:isConditionFulfilled("npc_lola", "after_sex")) then
        R:setTalkingActive(true)
    end

	R:setTilePosition(3,6)
	R:setFacingDown()
	R:setLooped(false)
	
end	