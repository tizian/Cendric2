-- Routine for NPC "King Logan III"

velocity = 150

loadRoutine = function(R, W)

    if (W:isConditionFulfilled("npc_loganthird","logan_gone")) then
        R:setDisposed()
        return
    end

    if (W:isConditionFulfilled("npc_loganthird","afraid")) then	
        R:setTalkingEnabled(false)
        R:setReloadEnabled(false)
        R:setLooped(false)
        R:setTilePosition(12.5, 5.25)
        R:goToTile(12.5, 7)
        R:goToTile(20, 7)
        R:goToTile(20, 3)
        R:setDisposedStep()
		W:addConditionProgress("npc_loganthird", "logan_gone")
        return
    end
    
    if (W:isConditionFulfilled("boss","BossRoyalguards")) then
        R:setTilePosition(12.5, 5.25)
        R:setTalkingActive(true)
        R:setLooped(false)
        R:setReloadEnabled(true)
		return
    end
    
    if (W:isConditionFulfilled("npc_loganthird","engage")) then
        R:setTilePosition(12.5, 5.25)
        R:setLooped(false)
        R:setReloadEnabled(true)
		return
    end
    
    R:setTilePosition(12.5, 5.25)
    R:setReloadEnabled(true)
    R:setLooped(false)
    R:setFacingDown()

end	