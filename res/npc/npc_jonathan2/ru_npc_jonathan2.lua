-- Routine for NPC "Jonathan" in the marshlands

velocity = 30

loadRoutine = function(R, W)

    if (not (W:isQuestState("element_master", "started") and W:isQuestComplete("element_master"))) then
        R:setDisposed()
        return
    end

	
	R:setTilePosition(7.5,31)
    R:setFacingDown()
	R:setTalkingActive(true)	
    R:setLooped(false)

end	