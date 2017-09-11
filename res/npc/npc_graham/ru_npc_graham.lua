-- Routine for NPC "Graham" in Gandria
loadRoutine = function(R, W)
	
	R:setTilePosition(16,2.75)
    
    if (not W:isConditionFulfilled("npc_graham", "talked")) then
        R:setTalkingActive(true)
    end
	
    R:wait(7000)
    R:goToTile(16, 3.5)
	R:goToTile(13, 3.5)
    R:setFacingUp()
    R:wait(2000)
    R:goToTile(18, 3.5)  
    R:wait(7000)
    R:goToTile(16, 3.5)
    R:goToTile(16, 2.75)

end	