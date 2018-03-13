-- Routine for NPC "Robert" in the meadows

velocity = 30

loadRoutine = function(R, W)

    if (W:isConditionFulfilled("npc_robert2", "gone")) then
        R:setDisposed()
        return
    end

    R:setReloadEnabled(true)
    if (W:isQuestState("teleport_robert", "completed")) then
        W:playSound("res/sound/tile/teleport.ogg")
        W:addConditionProgress("npc_robert2", "gone");
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
        R:setTalkingActiveForce(true)	
    end

end	