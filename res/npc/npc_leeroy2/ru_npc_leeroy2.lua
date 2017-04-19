-- Routine for NPC "Leeroy" after completing his quest.

loadRoutine = function(R, W)

    if (not W:isConditionFulfilled("default","swampbridge_open")) then
		R:setDisposed()
        return
	end
    
    if (not (W:isQuestState("save_leeroy","started") and W:isQuestComplete("save_leeroy"))) then
		R:setDisposed()
        return
	end

	R:setTalkingActive(true)
	R:setTilePosition(27,16)
	R:setFacingUp()
	R:setLooped(false)
    
    R:goToTile(27, 2)
    R:wait(1000)
    R:setDisposedStep()
	
end	