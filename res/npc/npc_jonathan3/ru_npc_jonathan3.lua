-- Routine for NPC "Jonathan" in Gandria

velocity = 30

loadRoutine = function(R, W)

    if (W:isQuestState("jonathan_knowledge", "void")) then
        R:setDisposed()
        return
    end
    
    if (W:isQuestState("jonathan_knowledge", "completed") and W:isConditionFulfilled("default","gandriacrypt_open")) then
        R:setDisposed()
        return
    end

	R:setTilePosition(45,6)
    R:setFacingDown()
    
    if (not W:isConditionFulfilled("npc_jonathan3", "talked")) then
        R:setTalkingActive(true)	
    end
    R:setLooped(false)

end	