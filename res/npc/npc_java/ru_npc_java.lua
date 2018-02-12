-- Routine for NPC "Java" 

velocity = 40

loadRoutine = function(R, W)

	if (W:isConditionFulfilled("npc_java","talked")) then
		R:setTalkingActive(false)
		R:setReloadEnabled(false)
	else
		R:setTalkingActive(true)
		R:setReloadEnabled(true)
	end
    
    R:setTilePosition(10,27)
        
    R:setFacingDown()
    R:wait(3000)
    R:goToTile(10,28)
    R:goToTile(13,28)
    R:wait(5000)
    R:goToTile(10,28)
    
    R:goToTile(10,27)

end	