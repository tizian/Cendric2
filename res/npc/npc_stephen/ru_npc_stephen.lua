-- Routine for NPC "Stephen"

loadRoutine = function(R, W)

	if (not W:isConditionFulfilled("npc_stephen", "talked")) then 
	
		R:setTalkingActive(true) 
		R:setLooped(false)
		R:setTilePosition(4,61.7)
		
		R:setReloadLocked(true)
		
		R:goToTile(4,63)
		R:goToTile(4,52.5)
		R:goToTile(21.5,52.5)
		R:goToTile(21.5,42)
		R:goToTile(21.5,42)
		R:goToTile(19,42)
		R:goToTile(19,23)
		R:setFacingDown()
		
		ReloadLocked(false)

	else
	
		R:setLooped(false)
		R:setTilePosition(19,23)
		R:setFacingDown()
		
	end 

end	