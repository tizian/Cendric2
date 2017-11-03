-- Routine for NPC "Velius"

velocity = 50

loadRoutine = function(R, W)
	
	R:setTilePosition(12.5,8)
	R:setReloadEnabled(false)
    R:setLooped(false)
    
    R:setTalkingActive(not W:isConditionFulfilled("npc_velius", "talked"))
	
	R:setFacingDown()

end	