-- Routine for NPC "Cynthia"

velocity = 50

loadRoutine = function(R, W)

    if (not W:isConditionFulfilled("npc_cynthia", "talked")) then 
		R:setTalkingActive(true) 
	end 
    
    R:setTilePosition(34,52.5)
    
    R:goToTile(49.5,52.5)
    R:goToTile(49.5,43.5)
    R:goToTile(34,43.5)
    R:goToTile(34,52.5)
    
    R:goToTile(34,58.5)
    R:setFacingDown()
    R:wait(5000)

    R:goToTile(34,52.5)

end	