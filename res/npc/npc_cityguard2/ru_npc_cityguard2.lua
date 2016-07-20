-- Routine for NPC "City Guard (2)"

loadRoutine = function(R, W)

	R:setTilePosition(67.9,59.7)
	R:setTalkingActive(not W:isConditionFulfilled("npc_cityguard","talked"))
	R:setLooped(false)
	
end	