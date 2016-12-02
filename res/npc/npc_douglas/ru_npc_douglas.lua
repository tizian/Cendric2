-- Routine for NPC "Douglas"

loadRoutine = function(R, W)

	R:setTilePosition(26.5,41)
	R:setTalkingActive(not W:isConditionFulfilled("npc_douglas","talked"))
	R:setLooped(false)
	
end	