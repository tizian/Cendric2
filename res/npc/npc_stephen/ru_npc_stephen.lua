-- Routine for NPC "Stephen"

loadRoutine = function(R, W)

	if (not W:isConditionFulfilled("npc_stephen", "talked")) then 
	
		R:setTalkingActive(true) 
		R:setLooped(false)
		R:setReloadEnabled(true)
		R:setTilePosition(4,61.7)
		R:goToTile(4,63)

	else 
	
		R:setReloadEnabled(false)

		R:goToTile(4,52.5)
		R:goToTile(21.5,52.5)
		R:goToTile(21.5,42)
		R:goToTile(21.5,42)
		R:goToTile(19,42)
		R:goToTile(19,24)
		R:goToTile(13,24)
		R:goToTile(13,23.5)
		R:setDisposed()

	end 

end	