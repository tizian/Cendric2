-- Routine for NPC "Rappa" 

velocity = 40

loadRoutine = function(R, W)

	if (W:isConditionFulfilled("npc_rappa","gone") or W:isConditionFulfilled("default","gandria_open")) then
		R:setDisposed()
        return
	end
    
    R:setTilePosition(10,6.5)
        
    R:setFacingUp()
    R:wait(3000)
    R:goToTile(6,6.5)
    R:setFacingUp()
    R:wait(3000)
   
    R:goToTile(10,6.5)

end	