-- Routine for NPC "Stephen"

loadRoutine = function(R, W)

	if (not W:isConditionFulfilled("npc_stephen", "talked")) then 
		R:setTalkingActive(true) 
	end 
	R:setTilePosition(4,62.7)
	R:goToTile(4,64)
	R:setLooped(false)
	
end	