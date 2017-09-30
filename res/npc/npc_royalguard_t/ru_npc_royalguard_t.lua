-- Routine for NPC "Royalguard (T)"

loadRoutine = function(R, W)

    if (W:isConditionFulfilled("boss","BossRoyalguard")) then
        R:setDisposed()
		return
    end

	if (not W:isConditionFulfilled("npc_royalguard_t", "talked")) then 
	
		R:setTalkingActive(true) 
		R:setLooped(false)
		R:setReloadEnabled(true)
		R:setTilePosition(12, 20)
		R:goToTile(12.5,20)
        R:goToTile(12.5,24)

	elseif (not W:isConditionFulfilled("default", "king_open")) then
	
		R:setTilePosition(12, 20)
        R:setReloadEnabled(true)
        R:setLooped(false)
        R:setFacingDown()
		
    else
    
        R:setReloadEnabled(false)
        R:setTilePosition(10, 7)
        R:setLooped(false)
        R:goToTile(12, 7)
		R:goToTile(10, 7)
        R:setFacingDown()

	end 

end	