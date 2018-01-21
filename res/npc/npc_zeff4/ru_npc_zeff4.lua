-- Routine for NPC "Zeff" in the last room
loadRoutine = function(R, W)

	if (not W:isConditionFulfilled("npc_zeff", "zeff_gone") or not W:isConditionFulfilled("boss", "BossVelius")) then
		R:setDisposed()
		return
	end
    
    if (not W:isConditionFulfilled("npc_zeff4", "talked")) then
        R:setTalkingActive(true)
    end
    

	R:setTilePosition(12.5, 16)
	
	R:goToTile(12.5,12)
    R:setLooped(false)

end	