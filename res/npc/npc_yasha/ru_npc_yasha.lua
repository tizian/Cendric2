-- Routine for NPC "Yasha"

velocity = 30

loadRoutine = function(R, W)

    if (W:isConditionFulfilled("boss", "yasha")) then
        R:setDisposed()
        return
    end

	
	R:setTilePosition(12,14.5)
    R:setFacingDown()
    
    if (not W:isConditionFulfilled("yasha", "talked")) then
        R:setTalkingActive(true)	
    end
    R:setLooped(false)

end	