-- Routine for NPC "Velius"

velocity = 50

loadRoutine = function(R, W)

    if (W:isConditionFulfilled("boss", "BossVelius")) then
        R:setDisposed()
        return
    end
	
	R:setTilePosition(12.5,8)
	R:setReloadEnabled(false)
    R:setLooped(false)
    
    R:setTalkingActive(not W:isConditionFulfilled("npc_velius", "talked"))
	
	R:setFacingDown()

end	