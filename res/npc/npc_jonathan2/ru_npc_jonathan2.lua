-- Routine for NPC "Jonathan" in the marshlands

velocity = 30

loadRoutine = function(R, W)

    if (not W:isQuestComplete("element_master")) then
        R:setDisposed()
        return
    end

	
	R:setTilePosition(7.5,31)
    R:setFacingDown()
    
    if (not W:isConditionFulfilled("npc_jonathan2", "talked")) then
        R:setTalkingActive(true)	
    end
    R:setLooped(false)

end	