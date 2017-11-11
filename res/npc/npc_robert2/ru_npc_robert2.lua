-- Routine for NPC "Robert" in the meadows

velocity = 30

loadRoutine = function(R, W)

    R:setReloadEnabled(true)
    if (W:isQuestState("teleport_robert", "completed")) then
        R:setDisposed()
        return
    end

	if (W:isConditionFulfilled("default", "plateau_open")) then
        R:setTilePosition(66,30)
        
        R:goToTile(66,30)
        R:wait(1000)
        R:goToTile(63,30)
        R:wait(1000)
    else
        R:setTilePosition(60,35.5)
        R:setFacingLeft()
        R:setLooped(false)
    end
    
    if (not W:isConditionFulfilled("npc_robert2", "talked")) then
        R:setTalkingActive(true)	
    end

end	