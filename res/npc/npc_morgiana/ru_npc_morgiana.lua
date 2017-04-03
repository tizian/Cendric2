-- Routine for NPC "Morgiana"

velocity = 50

loadRoutine = function(R, W)

    if (W:isConditionFulfilled("boss","BossMercenaries")) then
		R:setDisposed()
		return
	end

	R:setTilePosition(19,5)
    
    R:setFacingRight()
    R:wait(10000)
    R:goToTile(20,5)
    R:goToTile(20,7)
    R:goToTile(18,7)
    R:goToTile(18,11)
    R:goToTile(15,11)
    R:goToTile(15,10)
	R:setFacingDown()
	
    R:wait(10000)
    R:goToTile(15,11)
    R:goToTile(18,11)
    R:goToTile(18,7)
    R:goToTile(20,7)
    R:goToTile(20,5)
    R:goToTile(19,5)
	
end	