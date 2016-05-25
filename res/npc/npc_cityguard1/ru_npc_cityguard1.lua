-- Routine for NPC "City Guard (1)"

loadRoutine = function(R)

	R:setTilePosition(66,59.7)
	R:setTalkingActive(not R:isConditionFulfilled("npc_cityguard","talked"))
	R:setLooped(false)
	
end	