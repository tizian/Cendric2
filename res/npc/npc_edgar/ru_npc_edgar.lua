-- Routine for NPC "Overseer Edgar"

loadRoutine = function(R, W)

	if (not W:isConditionFulfilled("npc_edgar", "talked")) then 
		R:setTalkingActive(true) 
		R:setTilePosition(18,8.9)
		R:goToTile(18,9)
	else
		R:setTilePosition(14,4)
		R:goToTile(14,4)
		R:goToTile(13,4)
		R:goToTile(13,4.1)
	end 

	R:setLooped(false)
	
end	