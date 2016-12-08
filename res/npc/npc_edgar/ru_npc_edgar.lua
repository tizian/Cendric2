-- Routine for NPC "Overseer Edgar"

loadRoutine = function(R, W)

	if (not W:isConditionFulfilled("npc_edgar", "talked")) then 
		
		R:setTalkingActive(true) 
		R:setTilePosition(18,7)
		R:setReloadEnabled(false)
		R:goToTile(18,9)
		R:goToTile(14,4)
		R:goToTile(13,4)
		R:setFacingDown()
		R:setReloadEnabled(true)
		
	else
	
		R:setTalkingActive(false) 
		R:setTilePosition(13,4)
		R:setFacingDown()
		
	end 

	R:setLooped(false)
	
end	