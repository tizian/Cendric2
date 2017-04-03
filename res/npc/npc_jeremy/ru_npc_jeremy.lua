-- Routine for NPC "Jeremy"

velocity = 50

loadRoutine = function(R, W)

    if (W:isConditionFulfilled("boss","BossMercenaries")) then
		R:setDisposed()
		return
	end
    
	R:setTilePosition(20,4)
    
	R:setFacingDown()
    R:wait(11000)
    R:goToTile(20,5)
    R:goToTile(20,7)
    R:goToTile(18,7)
    R:goToTile(18,11)
    R:goToTile(16,11)
	R:setFacingLeft()
	
    R:wait(11000)
    R:goToTile(15,11)
    R:goToTile(18,11)
    R:goToTile(18,7)
    R:goToTile(20,7)
    R:goToTile(20,5)
    R:goToTile(20,4)
	
end	