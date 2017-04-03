-- Routine for NPC "Roy"

velocity = 45

loadRoutine = function(R, W)
    
    if (W:isConditionFulfilled("boss","BossMercenaries")) then
		R:setDisposed()
		return
	end
    
	R:setTilePosition(21,5)
    
	R:setFacingLeft()
    R:wait(9000)
    R:goToTile(20,5)
    R:goToTile(20,7)
    R:goToTile(18,7)
    R:goToTile(18,11)
    R:goToTile(15,11)
    R:goToTile(15,12)
	R:setFacingUp()
	
    R:wait(9000)
    R:goToTile(15,11)
    R:goToTile(18,11)
    R:goToTile(18,7)
    R:goToTile(20,7)
    R:goToTile(20,5)
    R:goToTile(21,5)
	
end	