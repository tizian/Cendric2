-- Routine for NPC "Alonso"

loadRoutine = function(R, W)

	R:setTilePosition(26.5,37)
	R:setTalkingActive(not W:isConditionFulfilled("npc_alonso","talked"))
	R:setLooped(false)
	
end	