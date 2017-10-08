-- Routine for NPC "Royalguard (I)"

loadRoutine = function(R, W)

    if (W:isConditionFulfilled("boss","BossRoyalguards")) then
        R:setDisposed()
		return
    end

	if (not W:isConditionFulfilled("default", "king_open")) then
	
		R:setTilePosition(13, 20)
        R:setReloadEnabled(true)
        R:setLooped(false)
        R:setFacingDown()
		
    else
    
        R:setReloadEnabled(false)
        R:setTilePosition(15, 7)
        R:setLooped(false)
        R:goToTile(13, 7)
		R:goToTile(15, 7)
        R:setFacingDown()

	end 

end	