-- Routine for NPC "Yasha"

velocity = 30

loadRoutine = function(R, W)

    if (W:isConditionFulfilled("boss", "BossYasha") and W:isConditionFulfilled("npc_yasha", "unfriendly")) then
        R:setDisposed()
        return
    end

	
	R:setTilePosition(12,13.5)
    R:setFacingDown()
    
    if (not W:isConditionFulfilled("npc_yasha", "talked")) then
        R:setTalkingActive(true)	
    end
    R:setLooped(false)

end	